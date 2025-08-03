# CLAUDE.md

このファイルは、このリポジトリでコードを作業する際のClaude Code (claude.ai/code) への指針を提供します。

## プロジェクト概要

Arduino UNO と L298N モータードライバを使用したNゲージ（鉄道模型）制御プロジェクトです。PWMによる速度制御と正転・反転の切り替えを実現し、クリーンアーキテクチャの設計思想を取り入れています。

## ハードウェア構成

- **Arduino UNO** + **L298N モータードライバモジュール**
- Nゲージ車両のDCモーター制御
- PWM速度制御（0-255の範囲）
- 正転・反転の方向制御
- 外部12V電源が必要

### ピン配置
- IN1: Arduino D8
- IN2: Arduino D9  
- ENA: Arduino D10 (PWM)
- OUT1/OUT2: レール・モーターに接続

## アーキテクチャ

クリーンアーキテクチャの原則に従い、関心事の明確な分離を行っています：

```
src/
├── domain/
│   └── MotorController.h      // インターフェース定義
├── usecase/
│   └── TrainControlUseCase.h  // ビジネスロジック層
├── infrastructure/
│   └── L298NMotorDriver.h     // ハードウェア実装
└── main.ino                   // エントリーポイント（依存性注入）
```

## 開発ノート

- **現在の状態**: リポジトリにはREADME.mdのみ - 実際のArduinoコード構造は計画済みだが未実装
- **言語**: 主要なドキュメントとコメントは日本語
- **ビルドシステム**: 標準的なArduino IDEワークフローを想定（package.json、Makefile、ビルドスクリプトなし）

## 主要機能

モーターコントローラーが提供する機能：
- `forward(speed)` - PWM速度制御による正転
- `reverse(speed)` - PWM速度制御による反転
- `stop()` - モーター停止

## 将来的な拡張計画

- ロータリーエンコーダーによるノッチ操作（段階的な加速・減速）
- リアルタイムディスプレイ連携
- 正転・反転の手動切替
- VVVF音の擬似生成
- Bluetooth/WiFiによる外部操作
- GUI連携

## 開発コマンド

正式なビルドシステムを持たないArduinoプロジェクトのため：
- コンパイルにはArduino IDEまたはPlatformIOを使用
- USB経由でArduino UNOに直接アップロード
- 特定のlint/testコマンドは未定義

## Communication Guidelines

- 基本的に日本語で受け答えしてください