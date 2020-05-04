# Ray Tracing The Next Week 学习笔记

## Motion Blur

> The basic idea is to generate rays at random times while the shutter is open and intersect the model at that one time. 

运动模糊，生活中很常见，就是物体在快门打开到关闭的短暂时间间隔内发生移动。相机发送射线，射线捕捉物体在随机快门时间间隔内线性插值的位移后的位置，多次射线采样达到运动模糊效果。

