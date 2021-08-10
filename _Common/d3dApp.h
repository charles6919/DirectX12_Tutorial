#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "d3dUtil.h"
#include "GameTimer.h"

//d3d12 라이브러리 연결
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

class D3DApp
{
protected:
	D3DApp(HINSTANCE hInstance);
	D3DApp(const D3DApp& rhs) = delete;
	D3DApp& operator=(const D3DApp& rhs) = delete;
	virtual ~D3DApp();

public:
	static D3DApp* GetApp();

	HINSTANCE	AppInst()const;
	HWND		MainWnd()const;
	float		AspectRatio()const;

	bool Get4xMsaaState()const;
	void Set4xMsaaState(bool value);

	int Run();

	virtual bool Initialize();
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	virtual void CreateRtvAndDsvDescriptorHeaps();
	virtual void OnResize();
	virtual void Update(const GameTimer& gt) = NULL;
	virtual void Draw(const GameTimer& gt) = NULL;

	virtual void OnMouseDown(WPARAM btnState, int x, int y) {};
	virtual void OnMouseUp(WPARAM btnState, int x, int y) {};
	virtual void OnMouseMove(WPARAM btnState, int x, int y) {};

protected:
	bool InitMainWindow();
	bool InitDirect3D();
	void CreateCommandObjects();
	void CreateSwapChain();

	void FlushCommandQueue();

	ID3D12Resource* CurrentBackBuffer()const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;

	void CalculateFrameStats();

	void LogAdapters();
	void LogAdapterOutputs(IDXGIAdapter* adapter);
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

protected:
	
	static D3DApp* mApp;  //싱글톤을 위한 정적포인터

	HINSTANCE mhAppInst = nullptr; // 응용 프로그램 인스턴스 핸들
	HWND      mhMainWnd = nullptr; // 메인 창 핸들
	bool      mAppPaused = false;  // 응용 프로그램 일시 정지 상태 여부
	bool      mMinimized = false;  // 응용 프로그램 최소화 여부  
	bool      mMaximized = false;  // 응용 프로그램 최대화 여부
	bool      mResizing = false;   // 사용자가 크기 조정용 테두리를 드래그 중인지 여부
	bool      mFullscreenState = false;// 전체화면 활성화 여부

	// true로 설정하면 4X MSAA (?.1.8).가 적용.  기본은 false이다.
	bool      m4xMsaaState = false;    // 4X MSAA 활성화 여부
	UINT      m4xMsaaQuality = 0;      // 4X MSAA 품질 수준

	// 경과 시간과 게임 전체 시간을 측정하는데 쓰인다.
	GameTimer mTimer;

	Microsoft::WRL::ComPtr<IDXGIFactory4>	mdxgiFactory;
	Microsoft::WRL::ComPtr<IDXGISwapChain>	mSwapChain;
	Microsoft::WRL::ComPtr<ID3D12Device>	md3dDevice;

	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
	UINT64 mCurrentFence = 0;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue>			mCommandQueue;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator>		mDirectCmdListAlloc;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>	mCommandList;

	static const int SwapChainBufferCount = 2;
	int mCurrBackBuffer = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
	Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;

	D3D12_VIEWPORT mScreenViewport;
	D3D12_RECT mScissorRect;

	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;

	// 파생 클래스는 자신의 생성자에서 이 멤버 변수들을 목적에 맞는 초기 값들로 설정해야 한다.
	std::wstring mMainWndCaption = L"d3d App";
	D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
	DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	int mClientWidth = 1920;
	int mClientHeight = 1080;

};