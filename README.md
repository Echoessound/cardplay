# Card Game (卡牌游戏)

基于 Cocos2d-x 开发的卡牌游戏项目，采用 MVC 架构设计。

## 项目简介

这是一个使用 Cocos2d-x 游戏引擎开发的卡牌游戏，支持多平台（Windows、Android、iOS、Linux）。游戏包含主牌堆区域和备用堆区域，玩家可以将符合条件的卡牌移动到备用堆，并支持撤销操作。

## 技术栈

- **游戏引擎**: Cocos2d-x
- **编程语言**: C++
- **平台支持**: Windows, Android, iOS, Linux

## 项目结构

```
cardpaly/
├── Classes/              # 游戏核心代码
│   ├── models/          # 数据模型层
│   │   ├── CardModel    # 单张卡牌状态
│   │   └── GameModel    # 游戏整体状态
│   ├── views/           # 视图层
│   │   ├── CardView     # 卡牌视图渲染
│   │   └── GameView     # 游戏主视图
│   ├── controllers/     # 控制器层
│   │   └── GameController # 游戏逻辑控制
│   ├── managers/        # 管理器
│   │   └── UndoManager  # 撤销/重做管理
│   ├── services/        # 服务层
│   │   └── GameModelFromLevelGenerator # 关卡生成服务
│   ├── configs/         # 配置相关
│   │   ├── models/      # 配置模型
│   │   └── loaders/     # 配置加载器
│   └── utils/           # 工具类
│       └── CardTextureHelper # 卡牌纹理辅助
├── Resources/           # 游戏资源
│   ├── res/            # 图片资源
│   └── fonts/          # 字体文件
├── cocos2d/            # Cocos2d-x 引擎源码
├── proj.win32/         # Windows 项目文件
├── proj.android/        # Android 项目文件
├── proj.ios_mac/        # iOS/macOS 项目文件
└── proj.linux/          # Linux 项目文件
```

## 架构设计

项目采用 **MVC (Model-View-Controller)** 架构模式：

### Model 层
- **CardModel**: 管理单张卡牌的状态（点数、花色、位置、是否栈顶等）
- **GameModel**: 管理整局游戏状态（所有卡牌、当前栈顶卡牌ID、备用堆位置等）

### View 层
- **CardView**: 负责单张卡牌的渲染和交互
- **GameView**: 负责游戏整体布局、动画播放和事件转发

### Controller 层
- **GameController**: 处理游戏逻辑、规则判断、状态更新和撤销管理

## 核心功能

1. **卡牌移动**: 支持将符合条件的卡牌移动到备用堆
2. **撤销功能**: 完整的撤销/重做机制，支持精确还原
3. **关卡系统**: 基于 JSON 配置的关卡加载
4. **动画系统**: 流畅的卡牌移动动画
5. **跨容器移动**: 支持卡牌在不同容器间移动并保持正确位置

## 编译说明

### Windows

使用 Visual Studio 打开 `proj.win32/cardpaly.sln` 进行编译。


## 扩展开发

### 添加新卡牌

1. 在关卡配置（`Resources/res/level_*.json`）中添加卡牌配置
2. 系统会自动加载并创建对应的 `CardModel` 和 `CardView`

### 添加新的撤销功能

1. 在 `UndoManager` 中定义新的撤销记录类型
2. 在 `GameController` 中实现对应的撤销逻辑
3. 在 `GameView` 中添加相应的动画支持

详细说明请参考 `程序设计文档.md`。

## 依赖项

- Cocos2d-x (已包含在项目中)
- CMake 3.6+


