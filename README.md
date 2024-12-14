# Retro Snake : Dual

Reference : [Cpp-Retro-Snake-Game-with-raylib](https://github.com/educ8s/Cpp-Retro-Snake-Game-with-raylib)

### Preparation

1. 下載 [final project](https://github.com/EnzoHuang0807/snake_cpp)
2. 下載 [raylib](https://www.raylib.com/)
3. 在 Vscode 打開 final project 所在的資料夾，輸入 Ctrl+Shift+\` 打開 powershell (不需要 WSL)
4. 執行以下指令，
```=
C:/raylib/w64devkit/bin/mingw32-make.exe RAYLIB_PATH=C:/raylib/raylib PROJECT_NAME=main OBJS=*.cpp
./main.exe
```
5. 開始玩

### 遊戲規則
![image](https://hackmd.io/_uploads/HJ5LwloVkl.png =300x)
* 地圖：25x25的方格，座標與二維陣列的索引相同 ([複習](https://hackmd.io/ST-iVaPlRqKrosaQgjGXkg))
* 玩家：分為黑蛇與白蛇
  * 黑蛇能用方向鍵控制
  * 白蛇能用 WASD 控制
* 食物 : 黑色十字
  * 蛇吃到食物時會變長一節
* 每局更新方式
    1. 黑蛇先移動
    2. 檢查黑蛇是否吃到食物
    3. 檢查黑蛇是否撞到牆壁、吃到自己或白蛇 (若有則重新開始，白蛇得一分)
    5. 白蛇移動
    6. 檢查白蛇是否吃到食物
    7. 檢查白蛇撞到牆壁、吃到自己或黑蛇  (若有則重新開始，黑蛇得一分)
* 獲勝方式
    最先獲得十分的一組獲勝




### What to do ?

用 C++ 撰寫一個遊戲腳本 (Game Script)，幫助你的蛇在遊戲中勝出

* 函式架構：
```cpp=
Direction setSnake(int foodPos[][2], int foodSize, 
    int snake1[][2], int snakeSize1, int snakeDirection1[2],
    int snake2[][2], int snakeSize2, int snakeDirection2[2]);
```

* 參數

| 名稱 | 功能 |
| ---- | ---- |
| `foodPos` | 二維陣列，紀錄食物的所在位置 |
| `foodSize` | 食物的數量 (`foodPos` 的長度) |
| `snake1` | 二維陣列，黑蛇所在的位置 |
| `snakeSize1` | 黑蛇的長度 (`snake1` 的長度) |
| `snakeDirection1` | 白蛇移動的方向 |
| `snake2` | 二維陣列，白蛇所在的位置 |
| `snakeSize2` | 白蛇的長度 (`snake2` 的長度) |
| `snakeDirection2` | 黑蛇移動的方向 |

* 回傳值

蛇的移動方向，`setSnake()` 需回傳 `UP` `DOWN` `LEFT` `RIGHT` 其中一個值
::: warning
回傳的方向會被主程式檢查，如果是違法操作則蛇會沿原本方向移動
:::

### 切換模式 (手動 / 腳本)

* 將黑蛇改由腳本控制 (修改`snake1.cpp`)
```=
C:/raylib/w64devkit/bin/mingw32-make.exe RAYLIB_PATH=C:/raylib/raylib PROJECT_NAME=main OBJS=*.cpp MODE_1=SCRIPT_1
./main.exe
```

* 將白蛇改由腳本控制 (修改`snake2.cpp`)
```=
C:/raylib/w64devkit/bin/mingw32-make.exe RAYLIB_PATH=C:/raylib/raylib PROJECT_NAME=main OBJS=*.cpp MODE_2=SCRIPT_2
./main.exe
```

### Schedule

* 12/15 (一)
    1. 預計分 2~3 組
    2. 討論並開始寫各組的遊戲腳本 
* 12/23 (一)
    1. 完成遊戲腳本 (需完成 `snake1.cpp` 與 `snake2.cpp`，邏輯可完全相同，改變數即可)
    2. 跟其他組 pk
    3. 跟我 pk (可能)

### 可以嘗試的方向

* 遇到障礙物要轉彎
* 往最近的食物移動
* ChatGPT (? 