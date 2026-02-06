#

### TObjectPtr

- UObject 멤버 변수를 안전하게 들고 있기 위한 UE5용 포인터 래퍼
- GC/리플렉션/직렬화(세이브, 로드, 에디터 노출) 흐름에 더 잘맞게 UObject 참조를 표현하는데 있다.

### 역할 1. UObject 강한 참조를 멤버로 표현

- UstaticMesh, UTexture2d, UActorComponent, AActor처럼 UObject계열을 멤버로 보관할 때 주로 사용
- 보통은 UPROPERTY()와 같이 써서 엔진이 이 UObject를 참조중임을 추적하게 됨
- 그래야 GC가 수거하면 안되는 객체를 제대로 판단하고, 에디터 Details노출, 직렬화 등도 정상 동작할 가능성이 높다.

### 역할 2. raw pointer(UObject\*)를 대체하는 UE5 표준 스타일

- UObject멤버 포인트는 TObjectPtr로 쓰자

### 어떤 경우에 사용하는가?

- Actor/Component/Asset을 클래스 멤버로 들고 있을때

#### 쓰지 않는경우

- 수명 연장업시 참조만하는 경우 : TWeakObjectPtr
- 에셋을 지금 로드하지않고 경로로만사용 : TSoftObjectPtr, TSoftClassPtr
- UObject가 아닌 일반 객체 : TUniquePtr / TSharedPtr (RAII영역)
