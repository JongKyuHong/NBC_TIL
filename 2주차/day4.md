# 언리얼

## 나만의 레벨 디자인 하기

### 주제 : 점프맵

- 간단한 점프맵을 만들어 볼 것 이다.
- learning kit에 있는 다양한 물체들을 점프대로 사용을 할 것이고, 움직이는 물체의 경우 SM_Module_Platform으로 만들 것이다.
- 물체가 혼자서 움직이는 것을 구현하고, 최대한 점프 조작감을 좋게하는것이 목표
- 가능하다면 오브를 만들어서 해당 오브를 먹으면 공중에서도 점프할 수 있는 기능을 만들 수 있으면 만들어 볼것이다.

![](https://velog.velcdn.com/images/kyu_/post/bea29310-d5b3-468a-8054-ac0fa44f043d/image.png)

### 설계

- new level만들기
- sky구성하기
- 시작지역, 종료지역 만들기
- 캐릭터 움직임, 애니메이션 구성하기
- 사용할 발판 에셋들 준비하고, 움직이는 발판, 아래로 떨어지는 발판은 bp클래스로 만들어 놓기
- bp eventgraph만들기
- 실제로 배치하기
- 종료지역에서 종료 이벤트 만들기
- 플레이 테스트하며 수정
  - 발판 밖으로 떨어졌을때 시작지점으로 돌아감

### 구현 과정

#### Land 애니메이션

착지하는 Land 애니메이션이 Run과 Idle로 있길래 속도에 따라 분기를 해주려고 했다.
달리는 상황이면 LandRun, 멈춰있는 상황이면 LandIdle
근데 anim graph에서 어떻게 분기를 하는지를 몰랐다. anim graph에는 branch가 없었다.

![](https://velog.velcdn.com/images/kyu_/post/1f446be0-9e1a-4fd1-b082-53c68a23427e/image.png)

Blend Poses by bool이라는 노드를 써서 해결하였다.
active value에 속도가 0보다 크다면이라는 조건을 주고 true pose와 false pose를 나눠서 노드에 꽂아주니 의도한대로 작동하였다.

![](https://velog.velcdn.com/images/kyu_/post/05b7a23d-0b8b-4aca-baec-0e1d59d6747f/image.png)

#### 떨어지는 발판

- 발판위에 착지한뒤 발판이 캐릭터의 무게를 못이기고 아래로 떨어지는 듯한 연출을 원했다.
- 평소처럼 beginoverlap 이벤트를 사용했고 테스트를 해보았는데 캐릭터가 착지해도 print string이 동작하지 않았다.

![](https://velog.velcdn.com/images/kyu_/post/e14ad498-114a-4957-ab8c-4331be04b963/image.png)

##### 콜리전 변경해보기

![](https://velog.velcdn.com/images/kyu_/post/8160bcd8-21df-45a1-99b4-9419434e2064/image.png)

collision preset을 custom으로 하고, pawn과 worlddynamic을 overlap으로 변경하고 실험해보았다.

![](https://velog.velcdn.com/images/kyu_/post/e3c0d486-7b6f-4c18-b824-d289aee74d37/image.gif)
print string은 잘뜨는데 내가 원하는 것처럼 발판위에 올라가지지는 않고 통과한다.

##### BeginOverlap에서 Component Hit으로 변경해보기

![](https://velog.velcdn.com/images/kyu_/post/4257b69c-63c2-434e-b9bb-b3816e569243/image.png)

노드를 Component Hit으로 변경해주고, 스태틱메시에서 Simulation Generates Hit Events를 체크해주었다.

![](https://velog.velcdn.com/images/kyu_/post/fd1c5e2a-2d03-4cfe-a4c0-472cee71b248/image.gif)

- 착지후 0.5초 딜레이 이후에 발판과 함께 떨어진다.
- 추후에 box collision + Beginplay방식으로 다시 변경했다.

#### 움직이는 발판

강의에서 배운대로 InterToMovement Component의 control값을 변경해서 설정하였다.

![](https://velog.velcdn.com/images/kyu_/post/3ce4dc67-8bc0-4be1-a6a3-180f5bd98ba6/image.gif)

![](https://velog.velcdn.com/images/kyu_/post/65ad6e5d-4ba0-41ce-8c4d-c4c70679f0e8/image.gif)

#### KillZone 설정

언리얼 내장 Kill Z를 쓰면 캐릭터가 destory되어서 box collision과 bp를 활용해서 kill zone을 만들었다. kill zone에 닿을시 시작 지점으로 리스폰되게 하려고 한다.

![](https://velog.velcdn.com/images/kyu_/post/44a6e36f-a072-4213-960d-62d35505e64f/image.png)

- BP_GameMode만들고 custom event를 만든다.
  - PlayerToRespawn이라는 BP_Character type의 Input을 만듬 (캐릭터가 하나밖에 없기때문에 BP_Character로 지정)
  - SpawnActor BP Character를 만들어서 리스폰할 위치를 지정하고, possess에 get palyer controller연결해서 조종할 수 있게 했다.
  - Destroy Actor로 기존 캐릭터 제거

![](https://velog.velcdn.com/images/kyu_/post/de613242-8810-4b70-9e0d-c4931f2cb104/image.png)

![](https://velog.velcdn.com/images/kyu_/post/130afa02-ee0e-45be-bc33-b7f8bb2ab860/image.gif)

#### 움직이는 발판2

움직이는 발판을 만들었을때 InterToMovement Component의 값을 지정해서 움직이게끔 만들어서 Horizontal Platform(좌우로 움직이는 발판)을 재사용하면 무조건 같은 방향으로 똑같이 움직여서 개선이 필요했다.

![](https://velog.velcdn.com/images/kyu_/post/d837cb7b-7478-40f1-8bda-4515a64ed2c1/image.gif)

우선 EndPosition(Vector), Duration(Float)이라는 변수를 만들고 EventGraph옆에 Construction Script탭으로 들어갔다.

##### Construction Script

- 블루프린트 액터가 생성되거나 수정될 때 자동으로 실행되는 특별한 이벤트 그래프
- 레벨에 블루프린트 배치, 변수 수정, 블루프린트 컴파일 시 실행됨
- 런타임에서는 Spawn Actor from Class로 동적 스폰할때 실행
- 게임 플레이 중 이미 배치된 액터는 Construction Script가 다시 실행되지 않음
- 인스턴스별로 다양성을 부여할때 사용
- 동적 생성시 초기화용

![](https://velog.velcdn.com/images/kyu_/post/a4c1f1c1-6873-4296-94e5-d46c6bb4d491/image.png)

![](https://velog.velcdn.com/images/kyu_/post/4e35f232-b12f-4e5f-ab0e-38d3a9198079/image.png)

- 변수들을 모두 public으로 뺐기때문에 viewport에서 변수만 수정해주면 내가 의도한대로 움직일 수 있었다.

![](https://velog.velcdn.com/images/kyu_/post/58809b90-1460-432e-a9e4-0f11510351ab/image.gif)

- ~~방향은 티가 안나지만~~
- 좌우 움직이는 발판이 방향과 속도가 다른 모습이다.

기존 움직이는 발판은 Horizontal, Vertical나눠있었는데 이것을 하나의 MovingPlatform이라는 블루프린트 클래스로 만들고 변수만 수정해서 재사용하기로 하였다.

![](https://velog.velcdn.com/images/kyu_/post/148bdb93-78b6-47a0-9334-67ed647be1c4/image.png)

![](https://velog.velcdn.com/images/kyu_/post/af46582e-0c29-49d3-96ce-8f819d9fcca6/image.png)

![](https://velog.velcdn.com/images/kyu_/post/6f76075e-7fd4-45ff-b044-9cb2bbdf7fde/image.png)

- 각자 타이밍과 방향이 다름

![](https://velog.velcdn.com/images/kyu_/post/d0d3a205-c9f6-45a8-9d48-8223e178df4e/image.gif)

- 하나의 MovingPlatform으로 재사용

#### 점프 오브 만들기

아이워너비더 시리즈를 보면 점프 오브가 존재한다.

- 예시 이미지
  ![](https://velog.velcdn.com/images/kyu_/post/6ffd957f-7dec-46db-aafa-f355bb6f14b7/image.png)

이 오브를 먹게 되면 공중에 있더라도 점프를 한번 더 할 수 있게되는데 이를 구현하고 싶었다.

- BP_JumpOrb 구현

  ![](https://velog.velcdn.com/images/kyu_/post/97ad49c6-77e8-4658-b23d-cfa1bb5759ab/image.png)

  - Sphere로 구슬을 만들었다.

- BP_Character에서 Jump 수정

  ![](https://velog.velcdn.com/images/kyu_/post/40ecb953-6c0b-4b4e-96cb-0990c956726a/image.png)

  - CanAirJump라는 bool변수를 하나 만들어서 공중에서 점프를 한번 더 할 수 있는지 즉, 오브를 먹었는지를 판단한다.
  - 만약 지상에서 점프를 하면 기존 점프가 됨
  - 점프중이거나 떨어질때 점프를 하면 분기를 타고 can air jump가 true인지 확인한다.(오브를 먹었는지 확인) 오브를 먹었다면 z축으로 420만큼 점프시켜주고 can air jump를 false로 초기화 해준다.

##### 오류

![](https://velog.velcdn.com/images/kyu_/post/84585df3-bb38-4c38-b8b6-3e80a47baf6f/image.png)

오브가 안없어지고 오브 위에 올라갈 수 있다.
-> orb의 collision이 BlockByAllDynamic으로 되어있었다. OverlapByAllDynamic으로 변경

![](https://velog.velcdn.com/images/kyu_/post/2c3d3f2c-c958-49cc-9e52-925c0ebe27e0/image.gif)

의도한대로 점프오브를 만들었다!
