#include "Server/Net/MOUIpNetDriver.h"

#include "Server/Chat/ChatTypes.h"   // LogMOUServer

std::atomic<int32> UMOUIpNetDriver::DesiredClientPort{ 0 };

void UMOUIpNetDriver::SetDesiredClientPort(int32 Port)
{
	// 범위를 벗어난 값은 0(엔진 기본)으로 떨어뜨린다. 여기서 이상한 값을 통과시키면
	// bind 가 실패하고 접속 자체가 안 되는데, 그 실패는 원인이 한참 뒤에 드러난다.
	const int32 Clamped = (Port > 0 && Port <= 65535) ? Port : 0;
	DesiredClientPort.store(Clamped);

	UE_LOG(LogMOUServer, Log, TEXT("[넷드라이버] 클라이언트 바인드 포트를 %s 로 설정했다."),
		Clamped == 0 ? TEXT("임시 포트(기본)") : *FString::FromInt(Clamped));
}

int32 UMOUIpNetDriver::GetDesiredClientPort()
{
	return DesiredClientPort.load();
}

int UMOUIpNetDriver::GetClientPort()
{
	const int32 Port = DesiredClientPort.load();
	if (Port == 0)
	{
		// 확보하지 못했다. 엔진 기본대로 임시 포트를 쓴다 —
		// 홀펀칭만 못 하고 접속 자체는 예전처럼 된다.
		return Super::GetClientPort();
	}

	UE_LOG(LogMOUServer, Log, TEXT("[넷드라이버] 클라이언트가 포트 %d 로 붙는다."), Port);
	return Port;
}
