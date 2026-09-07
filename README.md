# K60-game-console

基于 Freescale/NXP Kinetis K60 (MK60DZ10) 的自制掌上游戏机。100MHz 主频，使用 LQ12864 OLED 屏幕，通过 SD 卡加载资源，支持多款游戏与视频/音频播放。

## 硬件平台

| 模块 | 说明 |
|------|------|
| MCU | MK60DZ10，Core 100MHz / Bus 50MHz / Flex 50MHz |
| 屏幕 | LQ12864 OLED，128×64，SPI 驱动 |
| 输入 | 摇杆（ADC0 采集 X/Y）+ 确认/取消按键（PORTD） |
| 存储 | SD 卡 via SDHC + FatFs |
| 音频 | PWM/DAC 蜂鸣音 + VS1053 硬解 MP3（SPI0） |
| 调试 | UART0 @115200，调试 LED（PTC1） |

引脚与时钟参数见 [config/board_config.h](config/board_config.h)。

## 功能

主菜单（[jianmian.c](project/001-My%20NaoKe/app/jianmian.c)）提供以下条目：

1. **推箱子** — Sokoban，[tuixiangzi.c](project/001-My%20NaoKe/app/tuixiangzi.c)
2. **扫雷** — Minesweeper，[saolei.c](project/001-My%20NaoKe/app/saolei.c)
3. **五子棋** — Gomoku，[wuziqi.c](project/001-My%20NaoKe/app/wuziqi.c)
4. **Flash 播放器** — 从 SD 卡读取 `.bin` 帧序列播放（Bad Apple / Garnidelia），PIT0 按 20FPS 节拍刷新
5. **系统菜单** — 进入 [xtjianmian()](project/001-My%20NaoKe/app/jianmian.c)
   - **自定义地图绘制** — [huatu.c](project/001-My%20NaoKe/app/huatu.c)
   - **SD 卡容量查看** — `readsd()`

功能组件可通过 [config/feature_config.h](config/feature_config.h) 的宏开关一键裁剪（游戏 / 媒体 / 工具 / 框架服务 / 调试选项）。

> 注：WAV 软解播放（[wavplay.c](project/001-My%20NaoKe/app/wavplay.c)）实测失败，已在主程序中注释禁用。

## 目录结构

```
.
├── config/                板级硬件配置 / 功能裁剪开关
│   ├── board_config.h     引脚、时钟、外设参数（编译期固化）
│   └── feature_config.h   组件启停宏开关
├── framework/             基础框架（core / event / log / mem）
├── lib/                   依赖库
│   ├── CPU/               MK60DZ10 启动文件与系统时钟
│   ├── LPLD/              底层驱动（HW_* / DEV_*）
│   ├── FatFs/             文件系统
│   ├── USB/               USB 协议栈（CDC / HID）
│   ├── uCOS-II/           RTOS
│   └── common/            通用工具（queue / printf / memtest ...）
└── project/001-My NaoKe/  应用工程
    └── app/               主程序与各游戏实现
        ├── LPLD_FatFs.c   main() 入口、菜单调度、视频播放
        ├── input.*        摇杆边沿/连发、按键 ISR 置位、PIT 节拍
        ├── app_state.*    会话状态（替代散落的 extern 全局量）
        ├── LQ12864.*      OLED 驱动与绘图函数
        ├── yingjian.*     GPIO/ADC/PIT/PWM 初始化；ISR 只置标志
        └── *.c / *.h      各游戏与功能模块
```

## 输入与中断

- 按键 ISR 只置位，禁止 `delay` / LCD / 蜂鸣。
- 主循环调用 `input_poll()`，用 `input_edge()` 消费事件（带连发）。
- PIT1 每 10ms 累加时间；PIT0 按 20FPS 给出视频帧节拍。
- OLED 用 GPIO 位带写 SCL/SDA，避免每 bit 一次库函数 RMW。

## 构建

使用 IAR Embedded Workbench 打开 [LPLD_FatFs.eww](project/001-My%20NaoKe/iar/LPLD_FatFs.eww)，选择 `FLASH` 或 `RAM` 配置编译。工程自带 K60DN512 / K60DX256 两套链接配置（见 `lib/iar_config_files/`）。

SD 卡根目录需按 [LPLD_FatFs.c](project/001-My%20NaoKe/app/LPLD_FatFs.c) 中的路径放置资源：

- `0:/cartoon/badapple.bin` — Bad Apple 帧序列（86 宽）
- `0:/cartoon/jljt.bin` — Garnidelia 帧序列（114 宽）
