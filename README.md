UE5 制作 欧美 Solitaire 游戏

1：UE5 C++  写容易，一旦写错，空指针，编辑器直接崩溃。（开发效率大打折扣，只能在原有游戏模板上改改还行，或者大量使用蓝图） 

2： 使用MuMu 安卓模拟器，运行直接崩溃，初步排查，电脑硬件太差导致的，不支持Vulkan,和 DX12。

3：默认情况下，UE5 运行的时候，是在原有操作的视口下运行的，如果能像U3D一样 设置一个1080x1920 窗口，那就太好了

解决方法：播放按钮后面 有个按钮，这个按钮的形状是 3个竖着排列的点，点击后，选择 [新建编辑器窗口 PIE]，这样就能够使用 [编辑器偏好设置]里设置的 固定分辨率了

4: 默认新工程，UE5会加载所有 beta, 实验性插件，这个得手动调整一下。（卸载所有的beta/实验性 插件，重新启动UE Project 会直接崩溃）

5：GEngine->GetWorld() 一直是 nullptr, 奇怪！ (得用其他API) ，换成： GEngine->GameViewport->GetWorld();这个就有值。

6：Actor单例，无法使用泛型模板， 这样如何构造一个通用的单例基类，成为了一个问题。下面是我找到的解决办法:
	
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
				ensureMsgf(GEngine->GetWorld(), TEXT("GetActorSingleton<%s> GEngine->GetWorld() == null"), *Key->GetName());
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

	static TMap<TSubclassOf<AActor>, TWeakObjectPtr<AActor>> mInstanceDic;

UMG问题：
1：UMG 蓝图里 Widget 的【眼睛】设置，只在编辑模式下起作用，运行起来无效。得修改 详细面板里的 可视性 属性。


