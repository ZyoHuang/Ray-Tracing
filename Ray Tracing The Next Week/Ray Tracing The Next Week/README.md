# Ray Tracing The Next Week 学习笔记

## Motion Blur

> The basic idea is to generate rays at random times while the shutter is open and intersect the model at that one time. 

运动模糊，生活中很常见，就是物体在快门打开到关闭的短暂时间间隔内发生移动。相机发送射线，射线捕捉物体在随机快门时间间隔内线性插值的位移后的位置，多次射线采样达到运动模糊效果。





## Perlin Noise

**Perlin Noise**（柏林噪声）指由Ken Perlin发明的自然噪声生成算法 。

柏林噪声的特点：

- 同样的输入可以得到相同的输出，可复现性。
- 输入相邻的点可以获得相近的数字。
- 简单快捷，多用作hack。

### 平滑插值

原文使用**三线性插值（trilinear interpolation）**做平滑插值，然后用**埃尔米特平滑处理（Hermitian Smoothing）**再做平滑优化。

因为效果上看上去还是显得有点块状的噪点，所以另外采用了空间中一点的随机方向向量表示**梯度（gradient）**，即梯度向量。

------

在产生噪声纹理方面，我们通常用多个相加频率的复合噪声，被称为**Turbulence**。



### 参考资料

- [Simplex noise demystified](http://staffwww.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf)对Simplex noise进行详细的介绍——取自wiki