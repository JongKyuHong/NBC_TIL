# 팀프로젝트

- 우선 카메라 위치가 변경되었다.

![](https://velog.velcdn.com/images/kyu_/post/b6547bd0-5f5f-4c48-8e29-54b9f744350c/image.png)

- 살짝 오른쪽으로 카메라 위치를 변경

## 애니메이션 변경

- 기존에는 앉아서, 일어나서 총을 쏘면서 움직일때 움직이면서 총을 쏘는 애니메이션을 사용했다.
- 하지만 이렇게 사용하는경우 멈춘상태에서 총을 쏘고 움직이면 발이 땅에 고정된상태로 움직여지는게 보기 싫었다.
- 상하체를 분리해서 애니메이션을 적용하는방법이 있다고 들었어서 사용해보기로 하였다.

### Layered blend per bone 노드

![](https://velog.velcdn.com/images/kyu_/post/11c459bc-ec80-4dd4-9534-064a1bbf8efe/image.png)

- 캐릭터의 몸을 부위별로 나누어서 서로 다른 애니메이션을 합쳐줌
  - 자연스레 보이게 끔 만들어 준다.
- Base Pose : 기본 포즈, 캐릭터의 전체적인 움직임이 바탕이 되는 포즈
- Blend Pose : 특정 부위에만 덮어씌울 애니메이션
- 예를들면 총을 쏘면서 달려갈때 하체는 계속 움직이고, 상체는 공격 애니메이션을 진행

![](https://velog.velcdn.com/images/kyu_/post/9ec84460-2158-474e-9fab-05b854622d67/image.png)

- Branch Filter에서 어디서부터 섞을지 정할 수 있다.
  - 상하체 분리를위해 spine으로 설정했다.

### 버그발생

<iframe width="560" height="315"
src="https://www.youtube.com/embed/78kaoovSXGw"
title="YouTube video player" frameborder="0"
allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
allowfullscreen></iframe>

- ????
- 구르기의 경우 Main State안에 없고 Montage를 직접 호출하고 있었다.
- 이때 구르기 Montage를 FullBody슬롯을 만들어주어 거기에 넣고

![](https://velog.velcdn.com/images/kyu_/post/80adc593-ad32-423c-b34d-ce872d240a5d/image.png)

- Layered blend per bone 노드 후에 Full Body슬롯을 추가해서 해결하였다.

![](https://velog.velcdn.com/images/kyu_/post/cf48b96c-4e3b-420a-982c-8b1a5fbaddec/image.png)

<iframe width="560" height="315"
src="https://www.youtube.com/embed/GcgZbl_cUOs"
title="YouTube video player" frameborder="0"
allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
allowfullscreen></iframe>
