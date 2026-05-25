# 언리얼 C++

## 개발 환경 준비하기

### 폴더 구조 및 파일 설명

#### 주요 파일

- .sln : C++ 소스코드와 빌드 설정이 연결된 핵심 파일
- .uproject : 언리얼 에디터가 프로젝트 설정(콘텐츠, 앱, 플러그인 등)을 불러올 때 사용하는 파일
- .vsconfig : vs프로젝트에서 사용되는 특정 설정과 구성요소

#### 폴더

- .vs : vs가 자동으로 생성, 디버깅, 솔루션 설정등 관련 파일들이 있음
- Binaries : C++ 코드를 빌드하면 생성되는 실행파일 .exe, .dll등이 저장
- Config : 게임플레이, 렌더링, 입력 등의 설정을 담은 .ini 파일이 모여있다. 에디터나 코드로 특정 설정을 바꾸면, 이 폴더 내. ini 파일들이 종종 수정됨
- Source : C++ 소스 코드
- Content : 언리얼의 컨텐츠 브라우저와 연결되어 있다.
- DerivedDataCache : 에셋, 쉐이더등을 빠르게 처리하기 위한 `캐시 파일` 저장
- Intermediate : C++ 컴파일 과정에서 생성되는 임시 파일 및 빌드 중간 결과물이 저장
- Saved : 자동 저장 파일, 로그, 크래시 덤프등이 모여있음, 게임/에디터가 비정상 종료되었을때 이 폴더안의 로그 파일을 확인해 문제를 진단할 수 있다.

## 빌드 프로세스 이해하기

- c 파일 수정 -> 빌드 (컴파일 + 링크) -> DLL 파일 -> 언리얼 에디터
- DebugGame : 디버깅 최적화
- Development : 개발할때 주로사용, debug정보들은 최소화되어있음
- Shipping : 디버그 정보가 전부 빠진다. 가장 빠른 빌드, 출시되었을때 이 속도가 나오겠구나 > 마지막에 사용

### 솔루션 구조 내 주요 폴더

- Engine 폴더

  - 언리얼 엔진 자체 소스코드와 리소스가 담긴 곳
  - 에디터 작동, 엔진 코어 관련 코드가 포함되어 있다

- Games 폴더

  - 우리가 만든 프로젝트 코드가 들어있음

- Programs 폴더

  - 엔진 동작에 필요한 유틸리티 프로그램이나 서버 모듈이 담겨 있다.

- Rules 폴더

  - 엔진과 게임 등 각 모듈의 빌드 규칙을 정의해 놓은 파일들이 모여있다.

- Visualizers 폴더
  - vs 디버거에서 언리얼 엔진의 복잡한 자료구조를 보다 읽기 쉽게 시각화 할 수 있도록 도와주는 설정 파일

## 라이프 사이클 함수

### 생성자

- 오브젝트가 메모리에 생기면서 딱한번 호출

### PostInitializeComponents()

- 컴포넌트가 완성된 직후 호출, 컴포넌트끼리 데이터 주고받기, 상호작용

### BeginPlay()

- 배치 (Spawn) 직후

### Tick(float DeltaTime)

- 매 프레임마다 호출됨.
- 60프레임 기준 16ms

### Destroyed()

- 아이템을 먹고 사라질때를 생각해, 외부에서 나를 삭제시킬때
- 삭제되기 전에 호출

### EndPlay()

- 게임종료, 파괴(Destroyed()), 레벨 전환
- Destroyed를 호출하면 자연스레 EndPlay도 뒤따라옴 -> 갑자기 게임 종료가 되면 Destroyed는 호출되지않고 EndPlay만 호출

## 예시 코드

- Item.h

```c++
#pragma once

#include "CoreMinimal.h" // 로그함수, 언리얼에서 제공하는 기본 타입들 포함
#include "GameFramework/Actor.h" // 액터클래스에 대한 정보
#include "Item.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSparta, Warning, All); // 보통은 로그 헤더파일을 따로 만들어서 관리한다.

UCLASS()
class NBC1_API AItem : public AActor
{
	GENERATED_BODY()

public:
	AItem();

protected:
	USceneComponent* SceneRoot; // 씬
	UStaticMeshComponent* StaticMeshComp; // 스태틱메시

	virtual void BeginPlay() override;

};
```

- Item.cpp

```c++
// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

DEFINE_LOG_CATEGORY(LogSparta);

AItem::AItem()
{
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot")); // 씬루트에 USceneComponent 할당
    SetRootComponent(SceneRoot); // 루트 컴포넌트 지정

    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh")); // UStaticMeshComponent 할당
    StaticMeshComp->SetupAttachment(SceneRoot); // 스태틱메시 붙임


    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Resources/Props/SM_Star_B.SM_Star_B")); // 스태틱메시에서 Reference가져와서 경로 넣음, FObjectFinder를 사용
    if (MeshAsset.Succeeded())
    {
        StaticMeshComp->SetStaticMesh(MeshAsset.Object);

    }

    static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/Resources/Materials/M_Gem_C.M_Gem_C"));

    if (MaterialAsset.Succeeded())
    {
        // 머티리얼은 여러개를 적용할 수도 있다. 그래서 인자가 두개
        StaticMeshComp->SetMaterial(0, MaterialAsset.Object);
    }
}

void AItem::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("My Log!"));
    UE_LOG(LogSparta, Error, TEXT("Sparta"));
}
```

# 코드카타

## 문자 변환

- 숫자 짝꿍

```c++
#include <string>
#include <vector>

using namespace std;

string solution(string X, string Y) {
    string answer = "";
    int cntX[10] = {0};
    int cntY[10] = {0};

    for (char xChar : X){
        cntX[xChar-'0']++;
    }

    for (char yChar : Y){
        if (cntX[yChar-'0'] > 0){
            cntY[yChar-'0']++;
            cntX[yChar-'0']--;
        }
    }

    for (int i = 9; i >= 0; i--){
        answer.append(cntY[i], char('0'+i));
    }
    if (answer.length() == 0){
        answer = "-1";
    }
    if (answer[0] == '0'){
        answer = "0";
    }
    return answer;
}
```

- 문자열에 append라는 메서드를 배웠다.
- append(k, char) 문자열에 char형을 k번 붙일수있다.
- 이때 인덱스 i를 사용하였는데 char('0'+i); 형태로 많이 사용한다고 한다.
