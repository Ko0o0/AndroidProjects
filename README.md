# AndroidProjects

* ## 무엇을 ?
  > 안드로이드 해킹 방지를 위한 보안 솔루션 기능 직접 구현 및 우회

&nbsp;
* ## 어떻게 ?
  > 안드로이드 각종 기술에 대한 이해를 위해 예제 앱 개발을 시작으로  
  > 여러 예제 앱에서 사용된 기술을 합쳐 보안 솔루션 기능을 하는 하나의 앱 개발 예정  
  > 모든 기능은 웬만하면 JNI로 구현하려고 함.

&nbsp;
* ## 왜 ?
  > 직접 개발해보는 것이 관련 지식을 습득하는데 가장 좋다는 것을 깨달았고  
  > 내가 직접 개발한 앱을 리버싱해보면 사용 앱의 분석 포인트를 더 잘 파악할 수 있을 것이라고 생각하기 때문에..  

&nbsp;
* ## 구현 내용
  #### 루팅 탐지(Rooting Detection)  
  > 다양한 루팅 탐지 기법 파악 → Java에서 구현 → JNI에서 구현  
  > 
  #### 무결성 탐지(Integrity Detection)  
  > 솔직히 할지 안할지 모르겠음.. 일단 써놓음..  
  > 
  #### 디버깅 탐지(Anti Debugging)  
  > 다양한 디버깅 탐지 기법 파악 → Java에서 구현 → JNI에서 구현  
  >  
  #### 소스 코드 동적 로딩(Dynamic Loading DEX/SO File)  
  > Dynamic Loading DEX in Java → Dynamic Loading Encrypted DEX in Java → Dynamic Loading Encrypted DEX in JNI
  > Dynamic Loading SO in Java → Dynamic Loading Encrypted SO in Java → Dynamic Loading Encrypted SO in JNI (이것도 아직 생각만 하고 있음..)  
  
