-구현내용-
화면에는 넓은 바닥이 있습니다.
주인공 객체가 있고 중앙에서 시작합니다.
주인공 객체는 자동 이동을 하거나 직접 이동 방향을 정해줄 수 있습니다.
카메라는 X축 or Z축으로 이동하거나 화면의 중점에 대하여 회전 할 수 있습니다.
카메라는 주인공객체를 바라보고 있습니다.(시점 변환시에는 주인공 객체가 바라보는 방향을 바라봅니다.)
바닥에는 9개의 직육면체가 있으며 주인공 객체와 직육면체가 충돌하면 직육면체는 로봇으로 변하고 주인공 뒤를 따라옵니다.
카메라는 2가지 시점을 가지고 있습니다.
바닥의 꼭짓점에는 정육면체의 한 꼭짓점으로 서있고 제자리에서 자전하는 정육면체가 한개씩 있습니다.
조명을 적용했습니다.
객체가 한쪽 끝 지점까지가면 반대 끝 지점으로 나옵니다.

-키보드 명령-
s/S:  Z축으로 +방향 이동합니다.
w/W: Z축으로 -방향 이동합니다.
a/A: X축으로 -방향 이동합니다.
d/D: X축으로 +방향 이동합니다.
j/J: 주인공 객체가 점프합니다.
m/M: 주인공 객체가 자동이동합니다.
n/N: 시점을 변환합니다(2가지 시점)
t/T: 조명을 끄거나 켭니다.
X: 카메라를 X축 +방향으로 이동합니다.
x: 카메라를 X축 -방향으로 이동합니다.
Z: 카메라를 Z축 +방향으로 이동합니다.
z: 카메라를 Z축 -방향으로 이동합니다.
i/I: 카메라를 정지시킵니다.
f/F: 프로그램을 초기화합니다.
C: 카메라를 Y축 시계방향 회전합니다.
c: 카메라를 Y축 반시계방향 회전합니다.
q/Q 프로그램을 종료합니다.

-추가구현-
3인칭 쿼터뷰 외에 주인공객체 뒤에 카메라를 위치 시키는 구현을 했습니다.
m/M을 누르면 주인공 객체가 자동으로 랜덤한 방향으로 이동합니다.

