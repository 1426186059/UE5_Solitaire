<h1>UE5 制作 欧美 Solitaire(空当接龙纸牌游戏) [纯C++] 游戏</h1>

此空当接龙游戏，实现了小游戏制作的许多基础工具, 能够快速开发各种中小型游戏，包括Slots。

1： UE5 C++  
(1) 写容易，一旦写错，空指针，编辑器直接崩溃。（开发效率大打折扣，只能在原有游戏模板上改改还行，或者大量使用蓝图） 
(2) UE5 C++ 会自动忽略 未跑过的 任何方法代码块。(得打包的时候，才能看到所有的问题) (写的有多错，都不会报错，除非加到一个有逻辑的代码块里)
(3) 很多错误是链接失败错误：UE LiveCode 并不能提供更多信息，这个时候得关闭UE5，在VS中Build, 会有更多诊断信息出现。
(4): 不要对 FVector2D 等类似class 使用左值引用，一用C++就崩。 匿名函数，捕获变量，遇到FVector, 直接用[=,this]，而不能使用[&]
(5)： UE C++ for循环，加上break 直接报错。
(6) 纯C++ 类 (非UObject) 如果包含一个 UObject类的指针，UE5会自动GC这个字段。(野指针出现就崩), 比较典型的就是一个纯C++ 单例类， 包含一个 USaveGame* 指针。游戏运行10秒左右，直接被GC系统干掉为野指针。
或者把UObject* 用 TStrongObjectPtr<UObject> 表示，也能解决问题。
(7) UE5 C++ 默认不支持 typeid(T), 所以想用T 作为Map 的Key，得用其他路子。
(8) UE5 C++ Editor 和 运行时，用的是一个exe, 所以如果你在游戏中，用任何纯C++ 创建的任何单例，静态变量，PIE结束的时候，都不会销毁掉。(除非自己主动释放)，典型的例子：在游戏中注册了好多事件,PIE关闭没释放，下次开启PIE进游戏必崩。

2： 使用MuMu 安卓模拟器，运行直接崩溃，初步排查，电脑硬件太差导致的，不支持Vulkan,和 DX12。（后来 改为[发行] 打包就好了）

3：默认情况下，UE5 运行的时候，是在原有操作的视口下运行的，如果能像U3D一样 设置一个1080x1920 窗口，那就太好了

解决方法：播放按钮后面 有个按钮，这个按钮的形状是 3个竖着排列的点，点击后，选择 [新建编辑器窗口 PIE]，这样就能够使用 [编辑器偏好设置]里设置的 固定分辨率了

4: 默认新工程，UE5会加载所有 beta, 实验性插件，这个得手动调整一下。（卸载所有的beta/实验性 插件，重新启动UE Project 会直接崩溃）

5：GEngine->GetWorld() 一直是 nullptr, 奇怪！ (得用其他API) ，换成： GEngine->GameViewport->GetWorld();这个就有值。

6：Actor单例，无法使用泛型模板， 这样如何构造一个通用的单例基类，成为了一个问题。下面是我找到的解决办法:
	
	static TMap<TSubclassOf<AActor>, TWeakObjectPtr<AActor>> mInstanceDic;
    
    template<typename T>
	static T* GetActorSingleton(bool bForceCreate = true)
	{
		static_assert(TIsDerivedFrom<T, AActor>::Value, "T must be an AActor derived class");

		TSubclassOf<AActor> Key = T::StaticClass();
		TWeakObjectPtr<AActor>* mInstancePtr = mInstanceDic.Find(Key);
		if (mInstancePtr)
		{
			return Cast<T>(mInstancePtr->Get());
		}
		else
		{
			if (bForceCreate)
			{
				TWeakObjectPtr<AActor> mInstance = UEHelper::GetKKWorld()->SpawnActor<T>(Key);
				mInstanceDic.Add(Key, mInstance);
				return Cast<T>(mInstance.Get());
			}
			else
			{
				return nullptr;
			}
		}

	}

    void AddSingleton()
	{
		TSubclassOf<AActor> mKey = this->GetClass();
		if (!mInstanceDic.Contains(mKey))
		{
			mInstanceDic.Add(mKey, this);
		}
		else
		{
			TWeakObjectPtr<AActor>* mInstance = mInstanceDic.Find(mKey);
			if (mInstance->Get() != this)
			{
				UE_LOG(LogTemp, Error, TEXT("KKActorSingleton Error"));
			}
		}
	}

	void RemoveSingleton()
	{
		TSubclassOf<AActor> mKey = this->GetClass();
		mInstanceDic.Remove(mKey);
	}

7： Excel导出为CSV导入UE5, 最好的方法是使用UE5 的 UDataTable，否则各种乱码问题，不能一眼识别，运行直接崩。

UMG问题：

1：UMG 蓝图里 Widget 的【眼睛】设置，只在编辑模式下起作用，运行起来无效。得修改 详细面板里的 可视性 属性。

2: UMG 通过ZOrder 控制显示层级。

3: AddChildToCanvas/AddChild 方法 都会导致 孩子节点的锚点/对其/位置 不准，得重新设置。

4：通过 UButton->OnClicked.AddDynamic 第一次添加蓝图点击方法，某些时候，得重启UE5 才能起作用。(这其中涉及动态反射代码)



<h1>纯 C++ 问题</h1>

[我虽然看了C/C++代码有上百万行, 但我写C++代码,速度变的极慢，归根结底:还是C++实际工作经验少，平时只是写一些简单Demo。 另外不同的编译器，以及不同的C++书籍，都会导致不同的人理解C++ 不一样。]

1: 野指针： 比如自定义实现的 KKTween 类库，[内部]实现了一个 对象池，有最大容量。一旦超过最大容量，TweenItem* 堆对象直接被delete, 那么[外部]持有的 TwwenItem* 指针，立刻就成了野指针。这就是典型的内部管理生命周期，外部调用模式。一旦内部指针被销毁，外部的指针，立刻变为野指针。 怎么解决这个问题呢：花样很多:

(1) 对: 智能指针，外部只持有弱引用即可.

(2) 错: 全局Map索引：指针本质上是一个64位整数，可以当作Key, Value(随便一个bool值)，这样我们通过 是否包含Key来判断 是否为野指针。(这样做会存在问题，因为释放后的内存，会被其他new操作重复使用)

(3) 对: 还是全局Map 索引，但我们在对象中定义一个 64位整数作为静态变量，每分配一次，这个Id就加1，这样，我们拿这个Id 作为外部Handle的索引。 把Id 作为Map的Key, 指针作为Map的Value。

(4) 对：外部Handle订阅指针释放事件，我们在外部Handle中添加一个监听 delete（或加到析构函数里）的事件。这样也行。

(5) 禁止外部持有Handle，禁止内部自动管理生命周期。不要使用对象池，简单new,delete.

2: void foo(int); auto p1 = foo; auto p2 = &foo;  这p2 和p1 值一样



<h1>安卓打包问题</h1>

1： 打包 Pak: 2个重要目录: [额外资产目录]，[额外非资产目录]。

2：没有变为.uasset 的资源统称为非资产。是的，比如, 你不想使用 UDataTable，那么这些加入工程的，Json/CSV 都是非资产 (只在目录里有，但UE Editor 下，不可见)，非资产怎么进行打包呢：
是的，工程设置，打包选项里，专门有 [要打包的额外非资产目录] 我们点击后面的按钮，就可以打开文件浏览器，选择目录。

3：打包后发现手机上[屏幕适配]不对：所以得专门调一下[DPI适配曲线]的关键帧: 我参考的分辨率是(750, 1334) 那么我可以这样设置关键帧：(0, 0) => (7500, 10)， 或者[ScalToFit]。

4: UE5鼠标拖拽，在Android下会有问题，最大问题是：UWidgetLayoutLibrary::GetMousePositionOnPlatform() 这个位置，在Android下返回的有问题，导致无法更新位置。解决方案：
(1) 项目设置-引擎-输入 最下面 [鼠标用于触控] 选项勾上,（这样可以在PC上模拟Touch行为，也就是间接的调用了 Touch系列方法） 
(2) 然后使用 NativeOnTouchMoved 系列方法
(3) 不要使用这个API: UWidgetLayoutLibrary::GetMousePositionOnPlatform(). 而是 在NativeOnTouchMoved 里鼠标位置全局缓存一下，这样就可以在Tick里实时更新.

<h1>包体尺寸优化</h1>

1：游戏美术音效资源, 磁盘大小:75M， 导入UE后生成uasset文件资源: 120M。

2：未经其他优化： UE5.6开发包200M左右, 发行包140M

暂时没找到能更好压缩的方法，总体而言：UE5 API 和 U3D 出入不是很大。 

<h1>资源热更</h1>

1： 介绍两个重要热更工具:

(1)： RunUAT(Unreal Automation Tool) = 全流程大管家：编译、Cook、生成 PakList、调用 UnrealPak、出补丁、写 CI，全都管。RunUAT.bat BuildCookRun -project=xxx -platform=Win64 -build -cook -stage -pak -archive

(2)： UnrealPak = 专业打包机：只认列表，把文件打成 pak（或解包、挂载），别的不管。UnrealPak.exe DLC.pak -create=PakList.txt -compress


2：母包

(1) 母包默认是按照关卡Map引用的资源进行打包的, 但这种打包很脱离实际：比如我有很多武器类型，比如战仙一套，战神一套，战帝一套，那么这些图集Atlas很明显都不能在关卡中引用，所以[额外资产目录]这个目录的作用就显示出来了，这个目录，会强制UE把目录下的所有资源打包进母包。

3: 热更包:

(1)：添加系统环境变量：D:\ProgramFiles\Epic Games\UE_5.6\Engine\Binaries\Win64。

(2)：打包
