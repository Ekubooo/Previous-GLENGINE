## 实现说明：
# 将spotLight改变为随相机移动的可变光源
- 初始化时, spotLight 使用与相机相同的位置常量
- 在renderLoop中更新相机位置时后 更新spotLight位置与朝向
- 手电朝向依赖camera欧拉角, 注意输入shader的dir2Light相反于direction

# 缩小了模型，调整了MVP矩阵
- 将模型使用函数 glm::scale(model, vec3(0.1)) 缩小到0.1倍
- 模型矩阵兼顾位移与旋转(位置) 请复习101
- 如果加载多个模型，每个对象需要位置一个独立的model 矩阵

# model.cpp 纹理加载速度优化
- 加入纹理加载检测，用vector<Texture> textures_loaded 存储已加载纹理