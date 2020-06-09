Virtual RGB LED display

# 概要

[Controlling RGB LED display with Raspberry Pi GPIO](https://github.com/hzeller/rpi-rgb-led-matrix)
を実機がなくても開発できるようにする. LEDディスプレイのシュミレートができる.

# 準備

トップディレクトリで次のコマンドを実行する.
<!-- ただし,  `ffmpeg` と `timeout` が既に使える場合は不要です. -->
```
$ make setup
```

src/xxxx.cをコンパイルする場合は,
```
$ make xxxx
```
注) `$` はコマンドではない

# 実行

displayに引数(シュミレートする実行ファイル)を渡して実行
```
$ ./display main
```
`src/xxxx.c` の場合は `main` を `xxxx` に置き換える.

動画ファイルの保存先は `log/`

# プログラム

## LEDディスプレイのシュミレート

`struct RGBLedMatrix` の作成後 ( `led_matrix_create_from_options` を実行後) に必要
```c
pthread_t recthread;
if(pthread_create(&recthread, NULL, (void*)takeTimelapse, NULL)) {
    return -1;
}
```
`pthread` は `<pthread>` をインクルードする必要あり.

## パネルサイズ

シュミレータのLEDデフォルトサイズは64×64.
変更する場合は, 実行時の引数またはmatrix作成時に指定する.

この2つのサイズが異なる場合は, 実行時の引数が優先される.

### matrix生成時

```
struct RGBLedMatrixOptions options;
memset(&options, 0, sizeof(options));
options.rows = 32;
matrix = led_matrix_create_from_options(&options, &argc, (char***)&argv);
```

### 実行時

```
$ ./display main --led-rows=32
```
`=` の前後にスペースは入れられない.

## `include`

main関数で必要なライブラリ
```c
#include "led-matrix-c.h"   //いつも必要
#include "mnb_bmp.h"        //動画書出し時は必要
#include <pthread>          //動画書出し時は必要
```

## 関数

`include/led-matrix-c.h` に詳細があり, 実機LEDディスプレイで実行する場合はこれを参照する.

動画ファイルに書き出す場合は一部の挙動が違う or 未実装があり, 以下に記載する.

### 未実装

次に示す未実装の関数をソースコードに含める時, 実機用のライブラリをリンクする場合は動作するが,
動画書出し時は存在しない関数を呼び出していることになるので, リンク時にエラーになる(はず).

#### 未実装関数

* led_matrix_print_flags
* led_matrix_get_brightness
* led_matrix_set_brightness
* set_image
* load_font
* delete_font
* draw_text
* vertical_draw_text
* draw_circle
* draw_line

#### 未実装引数

`--led-cols` , `--led-rows` **以外**は無視する.

### 動作が違う関数

#### `led_matrix_create_from_options`

```c
struct RGBLedMatrix *led_matrix_create_from_options(
    struct RGBLedMatrixOptions *options,
    int *argc,
    char ***argv);
```
`options.rows`, `options.cols` **以外**は無視する.

#### `led_matrix_delete`

```c
void led_matrix_delete(
    struct RGBLedMatrix *matrix)
```
この関数を呼び出した後も `matrix` を使用でき, 動画の書出しもされる.

#### `led_matrix_swap_on_vsync`

```c
struct LedCanvas *led_matrix_swap_on_vsync(
    struct RGBLedMatrix *matrix,
    struct LedCanvas *canvas)
```
関数の返り値が入れ替え前の `matrix` ではなく, 意味のないポインタになる.
