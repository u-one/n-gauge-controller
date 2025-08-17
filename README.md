# n-gauge-controller

Arduino UNO + L298N を用いて、Nゲージ（鉄道模型）の走行を制御するプロジェクトです。  
PWMによる速度制御と、正転・反転の切り替えを実現。

---

## 🚆 機能概要

### 基本機能
- PWM による速度制御
- 正転 / 反転 の切り替え
- 常点灯
- 2行LCDによる状態表示

### 制御モード
- **MasconTrainController**: マスコン風制御（ロータリースイッチ + 方向切替）
- **DeviceTestController**: デバイステスト用（スイッチ状態表示）
- **SimpleTrainController**: シンプル制御
- **AutoTrainController**: 自動制御
- **DemoTrainController**: デモンストレーション用

---

## 🔧 使用ハードウェア

- Arduino UNO
- L298N モータードライバモジュール
- LCDキャラクタディスプレイ
  - みんラボ LCD3WIRE board
- ロータリースイッチ（車両制御用）
  - 1回路12接点
- トグルスイッチ（電源用）
- トグルスイッチ（方向切替用）
- 電源（例：12V 2A）
- DC モーター（Nゲージ車両）
- その他：レール、ジャンパワイヤ等

---

## 🔌 配線

| L298N端子 | 接続先               |
|-----------|----------------------|
| IN1       | Arduino D8           |
| IN2       | Arduino D9           |
| ENA       | Arduino D10（PWM）   |
| OUT1/OUT2 | レール（モーター）   |
| 12V       | 外部電源（例: 12V）  |
| GND       | Arduino GNDと共通    |
| 5V        | Arduino 5V（必要時） |

※ L298Nの 5V-ENジャンパーは通常そのままでOK。

### 詳細な配線図

- [配線図（ASCII版）](docs/wiring_diagram.md)
- [配線図（SVG版）](docs/wiring_diagram.svg)
- [Fritzing形式配線図](docs/fritzing_wiring.txt)

---

## 🧱 ディレクトリ構成

```
lib/
├── motor_controller/        # モーター制御インターフェース
│   └── src/
│       ├── MotorController.h
│       └── MotorController.cpp
├── rotary_switch/          # ロータリースイッチ制御
│   └── src/
│       ├── RotarySwitch.h
│       └── RotarySwitch.cpp
├── toggle_switch/          # トグルスイッチ制御
│   └── src/
│       ├── ToggleSwitch.h
│       └── ToggleSwitch.cpp
├── two_lines_display/      # 2行LCDディスプレイ制御
│   └── src/
│       ├── TwoLinesCharacterDisplay.h
│       └── TwoLinesCharacterDisplay.cpp
├── train_controller/       # 列車制御ロジック
│   └── src/
│       ├── TrainController.h          # 基底クラス
│       ├── TrainController.cpp
│       ├── MasconTrainController.h    # マスコン制御
│       ├── MasconTrainController.cpp
│       ├── DeviceTestController.h     # デバイステスト
│       ├── DeviceTestController.cpp
│       ├── SimpleTrainController.h    # シンプル制御
│       ├── SimpleTrainController.cpp
│       ├── AutoTrainController.h      # 自動制御
│       ├── AutoTrainController.cpp
│       ├── DemoTrainController.h      # デモ制御
│       └── DemoTrainController.cpp
└── LiquidCrystal3/         # LCD制御ライブラリ
    ├── LiquidCrystal3.h
    ├── LiquidCrystal3.cpp
    └── examples/

src/
└── main.cpp                # メインエントリーポイント
```

## ソフトウェア改善TODO
- 抽象レイヤ導入によるテスト容易性の向上
- シリアル出力によるデバッグ機能

---


## 🚀 将来的な拡張計画
- ✅ ロータリースイッチによるマスコン操作（実装済み）
- ✅ 2行LCDディスプレイによる状態表示（実装済み）
- ✅ 方向切替スイッチ（実装済み）
- 常点灯調整機能
- 制御モード切り替え機能
- VVVF音の擬似生成によるリアルな走行サウンドの演出
- BluetoothやWiFiによる外部操作
- より高機能なGUIとの連携
