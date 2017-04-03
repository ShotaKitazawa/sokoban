[About 'sokoban6.cc']

環境
* OS: CentOS 7.2
* Kernel: 3.10.0-327.36.3.el7.x86_64
* Compiler: g++ (GCC) 4.8.5 20150623 (Red Hat 4.8.5-4)
* CharacterCode: ASCII (LF)
* Editor: VIM - Vi IMproved 7.4

動作確認
* [g++ sokoban6 -l ncurses] でコンパイル
* 実行ファイルを実行後、数字でステージファイルを選択
** 不正な入力は cerr でエラー出力
** 選択したファイルのフォーマットが間違えていると ceer でエラー出力
* 矢印キーか h,j,k,l で上下左右移動
* q キーで終了
* b キーで一つ戻る
* r キーで初めから
* 終了判定可能
** 終了判定が真の場合、./etc/win.dat を読み込む
*** ./etc/win.dat が存在しない場合、std::cout で標準出力
