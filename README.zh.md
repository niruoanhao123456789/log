# Log

#### 介绍

本项目为一个基于 C++ 实现的高性能同步与异步日志系统，仅由头文件组成，无需第三方依赖，开箱即用。

其主要支持以下功能:

1. 支持多级别日志消息（DEBUG、INFOR、WARNING、ERROR、FATAL）
2. 支持同步日志与异步日志（异步模式下采用双缓冲 + 生产消费模型）
3. 支持多种日志落地方式：标准输出、指定文件、按大小滚动、按时间滚动
4. 支持多线程程序并发写日志，线程安全
5. 支持自定义格式化输出模式（`%d`、`%t`、`%p`、`%c`、`%f`、`%l`、`%m`、`%n` 等）
6. 支持通过继承 `LogSink` 基类扩展不同的日志落地目标
7. 支持局部日志器与全局日志器，全局日志器由 `LoggerManager` 单例统一管理

#### 软件架构

```
.
├── log/          # 日志库源码（仅头文件）
│   ├── Log.hpp           # 对外统一入口，提供宏代理接口
│   ├── Logger.hpp        # 日志器模块（同步/异步日志器、建造者、管理器）
│   ├── Level.hpp         # 日志级别定义
│   ├── LogMessage.hpp    # 日志消息封装
│   ├── Formatter.hpp     # 格式化器（解析格式化模式）
│   ├── Sink.hpp          # 日志落地模块（标准输出/文件/大小滚动/时间滚动）
│   ├── Buffer.hpp        # 异步双缓冲区实现
│   ├── Looper.hpp        # 异步工作线程（生产消费模型）
│   └── Util.hpp          # 通用工具类（时间、文件、目录）
├── example/      # 使用示例（功能测试）
├── extend/       # 扩展示例（按时间滚动落地演示）
└── bench/        # 性能测试（同步/异步多线程压测）
```

#### 核心技术

1. 类层次设计（继承和多态的应用）
2. C++20（多线程、auto、智能指针、右值引用等）
3. 双缓冲区 + 生产消费模型
4. 多线程与线程安全（互斥锁、条件变量、原子变量）
5. 设计模式（单例、工厂、代理、建造者、模板方法等）

#### 环境搭建

本项目不依赖任何第三方库，采用纯头文件实现。只需要具备以下环境即可编译运行：

- 支持 C++20 的编译器（如 g++）
- 链接时添加 `-pthread`
- 建议使用 Makefile 一键编译

编译示例：

```bash
cd example && make          # 编译使用示例
cd bench && make            # 编译性能测试
```

#### 使用说明

使用时通过 `#include "log/Log.hpp"` 引入头文件即可。

1. 使用默认根日志器直接输出（宏代理）：

```cpp
#include "log/Log.hpp"

LOGD("this is a debug: %d", 42);
LOGI("hello %s", "world");
LOGW("warning message");
LOGE("error message");
LOGF("fatal message");
```

2. 使用建造者模式构建自定义日志器：

```cpp
#include "log/Log.hpp"
using namespace LogModule;

std::unique_ptr<LoggerBuilder> builder = std::make_unique<GobalLoggerBuilder>();
builder->BuildLoggerName("MyLogger");
builder->BUildLoggerLevel(LogLevel::Level::DEBUG);
builder->BuildLoggerFormatter("[%d][%c][%f:%l][%p] %m%n");
builder->BUildLoggerSink<StdOutSink>();
builder->BUildLoggerSink<FileSink>("./logs/test.log");
builder->BUildLoggerSink<RollBySizeSink>("./logs/roll-", 10 * 1024 * 1024);
builder->BuildLoggerType(LoggerType::LOGGER_ASYNC);   // 或 LOGGER_SYNC
builder->Build();

Logger::ptr logger = GetLogger("MyLogger");
logger->error("%s", "custom logger works");
```

支持的格式化字符：

| 字符 | 含义 |
| :--- | :--- |
| `%d` | 日期时间（可跟 `{%Y-%m-%d %H:%M:%S}` 自定义格式） |
| `%t` | 线程 ID |
| `%p` | 日志级别 |
| `%c` | 日志器名称 |
| `%f` | 源文件名 |
| `%l` | 源码行号 |
| `%m` | 日志消息 |
| `%n` | 换行 |
| `%T` | 缩进（Tab） |

3. 落地方式说明：

- `StdOutSink`：输出到标准输出
- `FileSink`：输出到指定文件
- `RollBySizeSink`：按文件大小滚动（超过 `maxsize` 自动创建新文件）
- `RollByTimeSink`：按时间段滚动（秒/分钟/小时/天）
- 自定义落地：继承 `LogSink` 并重写 `Log()` 方法，通过 `BUildLoggerSink` 注册即可

4. 异步安全策略：`AsyncStatus::ASYNC_SAFE`（缓冲满则阻塞等待，默认）与 `AsyncStatus::ASYNC_UNSAFE`（缓冲满则扩容，用于极限测试）。

#### 目录说明

- `example/`：功能使用示例，演示同步/异步日志器、多种落地方式的构建与使用
- `extend/`：扩展示例，演示 `RollByTimeSink` 按时间滚动的使用
- `bench/`：性能测试，支持同步/异步、多线程、不同消息规模下的吞吐量测试

#### 参与贡献

1. Fork 本仓库
2. 新建 Feat_xxx 分支
3. 提交代码
4. 新建 Pull Request

#### License

本项目遵循 [LICENSE](LICENSE) 开源协议。
