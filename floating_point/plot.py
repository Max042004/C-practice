
import matplotlib.pyplot as plt
import numpy as np

# 讀取 log.txt
with open("log.txt", "r") as f:
    data = [float(line.strip()) for line in f if line.strip()]

arr = np.array(data)

# 計算中位數與平均值
median_val = np.median(arr)
mean_val = np.mean(arr)
max_val = np.max(arr)

print(f"med: {median_val}")
print(f"avg: {mean_val}")
print(f"max: {max_val}")

# 繪製折線圖
plt.figure(figsize=(12, 6))
plt.plot(arr, linestyle='-', color='blue', label="Numerical error")
plt.axhline(median_val, color='red', linestyle='--', label=f"Median = {median_val:.3f}")
plt.axhline(mean_val, color='green', linestyle='--', label=f"Average = {mean_val:.3f}")

plt.title("Numerical error plot")
plt.xlabel("pi/2 ~ inf")
plt.ylabel("numerical error")
plt.legend()
plt.grid(True)
plt.show()

