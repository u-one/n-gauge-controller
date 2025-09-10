import numpy as np
import matplotlib.pyplot as plt

# データ範囲を0-1000に設定
x = np.linspace(0, 2000, 1000)

# 線形増加（0-1000に比例して0-1000に増加）
linear_y = x

# 対数的増加（増加率がゆるやかになる）
# 対数関数を0-1000の範囲に正規化
log_y = 1000 * np.log(x + 1) / np.log(1001)

y_2 = -(x-1000)**2/1000+1000
y_3 = -(x-2000)**2/4000+1000

# グラフの作成
plt.figure(figsize=(10, 6))
plt.plot(x, linear_y, label='linear (y = x)', linewidth=2)
plt.plot(x, log_y, label='log (y = 1000 × log(x+1)/log(1001))', linewidth=2)
plt.plot(x, y_2, label='2', linewidth=2)
plt.plot(x, y_3, label='3', linewidth=2)




plt.xlabel('入力値 (0-1000)')
plt.ylabel('出力値 (0-1000)')
plt.title('線形増加 vs 対数的増加')
plt.legend()
plt.grid(True, alpha=0.3)
plt.xlim(0, 2000)
plt.ylim(0, 1000)

# グラフを表示
plt.tight_layout()
plt.show()

# データの一部を出力して確認
print("入力値 | 線形出力 | 対数出力")
print("-" * 30)
for i in [0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000]:
    linear_val = i
    log_val = 1000 * np.log(i + 1) / np.log(1001)
    print(f"{i:6d} | {linear_val:7.0f} | {log_val:7.0f}")