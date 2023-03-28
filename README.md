# 华为软件挑战赛2023 初赛

成渝赛区 64强

思路 贪心 + 参数优化

选取买入工作台$w_i$ 卖出工作台$w_j$ 使帧利润最大化

$f(w_i, w_j)=\frac{sell_i * factor(T_ij, 9000, 0.8) - buy_i + sell_j/num_j}{T_ij + T_i}$
