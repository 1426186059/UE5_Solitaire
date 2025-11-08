<h1>UE5 制作 欧美 Solitaire [纯C++] 游戏</h1>

1： UE5 C++  
(1) 写容易，一旦写错，空指针，编辑器直接崩溃。（开发效率大打折扣，只能在原有游戏模板上改改还行，或者大量使用蓝图） 
(2) UE5 C++ 会自动忽略 未跑过的 任何方法代码块。(得打包的时候，才能看到所有的问题) (写的有多错，都不会报错，除非加到一个有逻辑的代码块里)
(3) 很多错误是链接失败错误：UE LiveCode 并不能提供更多信息，这个时候得关闭UE5，在VS中Build, 会有更多诊断信息出现。
(4): 不要对 FVector2D 等类似class 使用左值引用，一用C++就崩。 匿名函数，捕获变量，遇到FVector, 直接用[=,this]，而不能使用[&]
(5)： UE C++ for循环，加上break 直接报错。
(6) 纯C++ 类 (非UObject) 如果包含一个 UObject类的指针，UE5会自动GC这个字段。(野指针出现就崩), 比较典型的就是一个纯C++ 单例类， 包含一个 USaveGame* 指针。游戏运行10秒左右，直接被GC系统干掉为野指针。
或者把UObject* 用 TStrongObjectPtr<UObject> 表示，也能解决问题。
(7) UE5 C++ 默认不支持 typeid(T), 所以想用T 作为Map 的Key，得用其他路子。

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

7： Excel导出为CSV, 正常保存就行了，别使用什么UTF8 啥的，否则 CSV 加载会有问题。

UMG问题：

1：UMG 蓝图里 Widget 的【眼睛】设置，只在编辑模式下起作用，运行起来无效。得修改 详细面板里的 可视性 属性。

2: UMG 通过ZOrder 控制显示层级。

3: AddChildToCanvas/AddChild 方法 都会导致 孩子节点的锚点/对其/位置 不准，得重新设置。

4：通过 UButton->OnClicked.AddDynamic 第一次添加蓝图点击方法，某些时候，得重启UE5 才能起作用。(这其中涉及动态反射代码)



<h1>纯 C++ 问题</h1>

[我虽然看了C/C++代码有上百万行，看的很流畅，很嗨。 但我写C++代码,速度变的极慢，归根结底:还是C++实际工作经验少，平时只是写一些简单Demo。 另外不同的编译器，以及不同的C++书籍，都会导致不同的人理解C++ 不一样。]

1: 野指针： 比如自定义实现的 KKTween 类库，[内部]实现了一个 对象池，有最大容量。一旦超过最大容量，TweenItem* 堆对象直接被delete, 那么[外部]持有的 TwwenItem* 指针，立刻就成了野指针。这就是典型的内部管理生命周期，外部调用模式。一旦内部指针被销毁，外部的指针，立刻变为野指针。 怎么解决这个问题呢：花样很多:

(1) 对: 智能指针，外部只持有弱引用即可.

(2) 错: 全局Map索引：指针本质上是一个64位整数，可以当作Key, Value(随便一个bool值)，这样我们通过 是否包含Key来判断 是否为野指针。(这样做会存在问题，因为释放后的内存，会被其他new操作重复使用)

(3) 对: 还是全局Map 索引，但我们在对象中定义一个 64位整数作为静态变量，每分配一次，这个Id就加1，这样，我们拿这个Id 作为外部Handle的索引。 把Id 作为Map的Key, 指针作为Map的Value。

(4) 对：外部Handle订阅指针释放事件，我们在外部Handle中添加一个监听 delete（或加到析构函数里）的事件。这样也行。

(5) 禁止外部持有Handle，禁止内部自动管理生命周期。不要使用对象池，简单new,delete.


<h1>安卓打包问题</h1>

1： 勾选：使用pak文件，勾选使用IO Store

2：没有变为.uasset 的资源统称为非资产。是的，比如, 你不想使用 UDataTable，那么这些加入工程的，Json/CSV 都是非资产 (只在目录里有，但UE Editor 下，不可见)，非资产怎么进行打包呢：
是的，工程设置，打包选项里，专门有 [要打包的额外非资产目录] 我们点击后面的按钮，就可以打开文件浏览器，选择目录。

