# Log

#### Introduction

This project is a high-performance synchronous and asynchronous logging system implemented in C++. It is header-only and requires no third-party dependencies, ready to use out of the box.

Its main supported features are as follows:

1. Support for multiple log message levels (DEBUG, INFOR, WARNING, ERROR, FATAL).
2. Support for both synchronous and asynchronous logging (asynchronous mode uses double buffering and a producer-consumer model).
3. Support for multiple output destinations: standard output, specified files, size-based rolling files, and time-based rolling files.
4. Support for concurrent log writing in multithreaded programs (thread-safe).
5. Support for custom formatting patterns (`%d`, `%t`, `%p`, `%c`, `%f`, `%l`, `%m`, `%n`, etc.).
6. Support for extending different log destination targets by inheriting the `LogSink` base class.
7. Support for local and global loggers, where global loggers are managed centrally by the `LoggerManager` singleton.

#### Software Architecture

```
.
├── log/          # Log library source (header-only)
│   ├── Log.hpp           # Unified entry point, provides macro proxy interfaces
│   ├── Logger.hpp        # Logger module (sync/async loggers, builder, manager)
│   ├── Level.hpp         # Log level definitions
│   ├── LogMessage.hpp    # Log message wrapper
│   ├── Formatter.hpp     # Formatter (parses formatting patterns)
│   ├── Sink.hpp          # Sink module (stdout / file / size-rolling / time-rolling)
│   ├── Buffer.hpp        # Async double-buffer implementation
│   ├── Looper.hpp        # Async worker thread (producer-consumer model)
│   └── Util.hpp          # Utility classes (time, file, directory)
├── example/      # Usage examples (functional tests)
├── extend/       # Extension examples (time-based rolling sink demo)
└── bench/        # Performance benchmarks (sync/async multi-thread stress tests)
```

#### Core Technologies

1. Class hierarchy design (application of inheritance and polymorphism).
2. C++20 (multithreading, auto, smart pointers, rvalue references, etc.).
3. Double buffering and the producer-consumer model.
4. Multithreading and thread safety (mutex, condition variables, atomic variables).
5. Design patterns (singleton, factory, proxy, builder, template method, etc.).

#### Environment Setup

This project does not depend on any third-party libraries and is implemented with header-only files. You only need the following to compile and run:

- A C++20-capable compiler (e.g., g++).
- Link with `-pthread`.
- A Makefile is provided for one-command builds.

Compile examples:

```bash
cd example && make          # Build usage examples
cd bench && make            # Build performance benchmarks
```

#### Usage Instructions

Simply include `log/Log.hpp` to start using it.

1. Log directly via the default root logger (macro proxy):

```cpp
#include "log/Log.hpp"

LOGD("this is a debug: %d", 42);
LOGI("hello %s", "world");
LOGW("warning message");
LOGE("error message");
LOGF("fatal message");
```

2. Build a custom logger with the builder pattern:

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
builder->BuildLoggerType(LoggerType::LOGGER_ASYNC);   // or LOGGER_SYNC
builder->Build();

Logger::ptr logger = GetLogger("MyLogger");
logger->error("%s", "custom logger works");
```

Supported formatting characters:

| Char | Meaning |
| :--- | :--- |
| `%d` | Date/time (optionally `{%Y-%m-%d %H:%M:%S}` for a custom format) |
| `%t` | Thread ID |
| `%p` | Log level |
| `%c` | Logger name |
| `%f` | Source file name |
| `%l` | Source line number |
| `%m` | Log message |
| `%n` | Newline |
| `%T` | Tab indent |

3. Output destination overview:

- `StdOutSink`: writes to standard output.
- `FileSink`: writes to a specified file.
- `RollBySizeSink`: rolling by file size (a new file is created once `maxsize` is exceeded).
- `RollByTimeSink`: rolling by time interval (second/minute/hour/day).
- Custom sink: inherit `LogSink` and override `Log()`, then register it via `BUildLoggerSink`.

4. Async safety policy: `AsyncStatus::ASYNC_SAFE` (blocking wait when the buffer is full, default) and `AsyncStatus::ASYNC_UNSAFE` (expand the buffer when full, intended for stress tests).

#### Directory Overview

- `example/`: functional usage examples demonstrating sync/async loggers and multiple sinks.
- `extend/`: extension examples demonstrating time-based rolling via `RollByTimeSink`.
- `bench/`: performance benchmarks measuring throughput for sync/async, multi-thread, and varying message sizes.

#### Contribution

1. Fork the repository.
2. Create a `Feat_xxx` branch.
3. Commit your code.
4. Create a Pull Request.

#### License

This project is licensed under the open-source license in [LICENSE](LICENSE).
