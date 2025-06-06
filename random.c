void setNonBlockingInput(int enable) {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);

    if (enable) {
        ttystate.c_lflag &= ~ICANON;  // バッファリング無効
        ttystate.c_lflag &= ~ECHO;    // 入力文字の表示無効
        tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    } else {
        ttystate.c_lflag |= ICANON;
        ttystate.c_lflag |= ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
        fcntl(STDIN_FILENO, F_SETFL, 0);
    }
}

int main() {
    const char* names[] = {
        "A","B","C","D","E","F","G","H",
        "I","J","K","L","M","N","O","P",
        "Q","R","S","T","U","V","W","X","Y","Z
    };
    int num_names = sizeof(names) / sizeof(names[0]);

    srand((unsigned int)time(NULL));
    printf("The roulette is spinning.....\n\n");

    setNonBlockingInput(1);  // 非ブロッキング入力有効

    int delay = 50000; // 初期スピード（50ms）
    int final_index = 0;
    int slowing_down = 0; // 減速開始フラグ
    int slowdown_counter = 0; // 減速回数（何ステップで止めるか）

    while (1) {
        final_index = rand() % num_names;
        printf("\r%-20s", names[final_index]);
        fflush(stdout);
        usleep(delay);

        if (!slowing_down && getchar() != EOF) {
            slowing_down = 1;
            slowdown_counter = 12;  // 減速ステップ数
        }

        if (slowing_down) {
            delay += 50000; // 徐々に遅くする
            slowdown_counter--;
            if (slowdown_counter <= 0) {
                break;
            }
        }
    }

    setNonBlockingInput(0);  // 非ブロッキング解除

    printf("\r%-20s\n", "");
    printf("ルーレットの結果: %s さん\n", names[final_index]);

    return 0;

 }
