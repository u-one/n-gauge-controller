# n-gauge-controller

Arduino UNO + L298N を用いて、Nゲージ（鉄道模型）の走行を制御するプロジェクトです。  
PWMによる速度制御と、正転・反転の切り替えを実現しており、  
クリーンアーキテクチャの設計思想を取り入れた構成を採用しています。

---

## 🔧 使用ハードウェア

- Arduino UNO
- L298N モータードライバモジュール
- DC モーター（Nゲージ車両）
- 電源（例：12V 2A）
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

---

## 🧱 ディレクトリ構成（クリーンアーキテクチャ）

```
src/
├── domain/
│ └── MotorController.h // インターフェース
├── usecase/
│ └── TrainControlUseCase.h // ビジネスロジック
├── infrastructure/
│ └── L298NMotorDriver.h // L298N具体実装
├── main.ino // setup/loop + DI
```

---

## 🚆 機能概要

- PWM による速度制御（0〜255）
- 正転 / 反転 の切り替え
- 停止処理
- テストコードで動作確認可能

---

## 🧪 テストシーケンス（例）

```cpp
motor->forward(100);   // 正転（速度100）
delay(2000);
motor->stop();
delay(1000);
motor->reverse(150);   // 反転（速度150）
delay(2000);
motor->stop();

---

## 将来的な拡張計画
- ロータリーエンコーダーによるノッチ操作（段階的な加速・減速）
- ディスプレイ接続による動作状況のリアルタイム表示
- 正転・反転の手動切替（エンコーダ・スイッチ等による）
- VVVF音の擬似生成によるリアルな走行サウンドの演出
- BluetoothやWIFIによる外部操作
- より高機能なGUIとの連携