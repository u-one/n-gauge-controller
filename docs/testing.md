# ユニットテスト実行ガイド

このドキュメントでは、Nゲージコントローラプロジェクトのユニットテストの実行方法と構成について説明します。

## テスト環境

- **テストフレームワーク**: Unity（PlatformIO標準）
- **実行環境**: Native（ホストマシン上でのテスト実行）
- **対象コード**: ハードウェア非依存の業務ロジック

## テスト構成

### 1. ハードウェア抽象化層

テストを可能にするため、ハードウェア操作を抽象化するインターフェースを導入しました：

- `IHardwareInterface`: ハードウェア操作の抽象インターフェース
- `ArduinoHardware`: 実際のArduino環境用実装
- `MockHardware`: テスト用モック実装

### 2. テスト対象クラス

#### TestableMotorController
元の`MotorController`をテスト可能にリファクタリングしたクラス：

- PWM周波数制御（30-2000Hz範囲）
- duty比制御（0-100%範囲）
- モーター方向制御（前進/後進/停止）
- VVVF音再現機能

**テスト項目:**
- 初期化処理の確認
- モーター方向制御（前進/後進/停止）
- PWM周波数設定と範囲制限
- duty比設定と範囲制限
- VVVF音再現用周波数範囲テスト

#### RotarySwitch
ロータリースイッチの状態判定ロジック：

- 12段階のスイッチ状態判定
- アナログ値からの状態変換
- 速度値と方向の決定

**テスト項目:**
- アナログ値からの正確な状態判定
- 許容誤差範囲での状態判定
- 範囲外値でのUnknown状態判定
- 状態から速度値への変換
- 状態から方向（前進/後進）への変換
- 統合テスト（アナログ値→状態→速度・方向）

## テスト実行方法

### 前提条件
- PlatformIOがインストールされていること
- プロジェクトルートディレクトリにいること

### 全テストの実行
```bash
pio test -e native
```

### 個別テストの実行
```bash
# MotorControllerのテストのみ
pio test -e native --filter test_motor_controller

# RotarySwitchのテストのみ  
pio test -e native --filter test_rotary_switch
```

### テスト結果の詳細表示
```bash
pio test -e native -v
```

## ファイル構成

```
├── lib/
│   ├── hardware_abstraction/src/
│   │   ├── IHardwareInterface.h       # ハードウェア抽象化インターフェース
│   │   ├── ArduinoHardware.h          # Arduino実装
│   │   └── MockHardware.h             # テスト用モック
│   ├── motor_controller/src/
│   │   ├── TestableMotorController.h  # テスト可能なモーターコントローラ
│   │   └── TestableMotorController.cpp
│   └── rotary_switch/src/
│       ├── RotarySwitch.h             # ロータリースイッチロジック
│       └── RotarySwitch.cpp
├── test/
│   ├── test_motor_controller/
│   │   └── test_motor_controller.cpp  # モーターコントローラテスト
│   └── test_rotary_switch/
│       └── test_rotary_switch.cpp     # ロータリースイッチテスト
└── platformio.ini                     # テスト環境設定
```

## テスト設計の特徴

### 1. ハードウェア非依存
- モックを使用してハードウェア操作をシミュレート
- 実際のArduinoボードなしでテスト実行可能
- GPIO状態やタイマーレジスタ値を内部で管理・検証

### 2. 境界値テスト
- PWM周波数の上限・下限テスト（30Hz〜2000Hz）
- duty比の範囲テスト（0-100%）
- ロータリースイッチのアナログ値境界テスト

### 3. 統合テスト
- アナログ値→スイッチ状態→速度・方向の一連の変換テスト
- PWM設定の統合テスト（周波数とduty比の同時設定）

## 期待される効果

1. **品質向上**: 業務ロジックの正確性を自動テストで保証
2. **リファクタリング安全性**: 変更時の影響を早期発見
3. **ドキュメント効果**: テストコードが仕様書の役割を果たす
4. **継続的改善**: CI/CD環境での自動テスト実行が可能

## 今後の拡張予定

- LCDディスプレイ制御のテスト
- エラー処理のテスト
- パフォーマンステスト
- 実機でのハードウェア統合テスト