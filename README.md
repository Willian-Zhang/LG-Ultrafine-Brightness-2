# LG-Ultrafine-Brightness 2

A tool to adjust brightness of LG Ultrafine 4k/5K on **Windows** without the help of Bootcamp.

The program simply send set feature report to the device via USB and then the LG 4/5K monitor adjust the brightness upon receiving such reports.

This project is derived from [https://github.com/csujedihy/LG-Ultrafine-Brightness](https://github.com/csujedihy/LG-Ultrafine-Brightness) which is derived from [https://github.com/unknownzerx/lguf-brightness](https://github.com/unknownzerx/lguf-brightness).

Thanks @csujedihy, @unknownzerx.

The original tool was unable to with simple option or hotkey activation, so here it is.

## Download
[Link to zip](https://github.com/Willian-Zhang/LG-Ultrafine-Brightness-2/releases/download/v2.5.1/brightness-2-5-1.zip)

## Usage
Just double click.

### Want more details?
Run application as `LG-Ultrafine-Brightness.exe <option>` (shortcut) or `LG-Ultrafine-Brightness.<option>.exe` (rename)
```
	Valid <option> arguments: `-` `+` `[` `]` `0` `9` `p` `h`
		'-' or '+' to adjust brightness.
		'[' or: ']' to fine tune.
		'p' to print infomation
		'0' ~ '9' to use the 0% thru 100% level brightness
		'h' to show this info
```

### Works best with
- [3RVX](https://3rvx.com/)
    - Example: 
        - settings - Hotkeys - "+" - Action - Run - <Path to `LG-Ultrafine-Brightness.+.exe` (duplicate, rename yourself)>
        - Press the hotkey you set, brightness changes! 🎉
- Or any other hotkey applications with `Run...` capabilities


## History 
- 2.5:
	- Optimized Speed
	- Option 0~9
- 2.1:
    - have it not poping up any window, on running the app, when there's no message to show up.
- 2.0:
    - this branch

## Development
### Build

1. Open the `sln` file by Visual Studio.
2. Select `x64` configuration.
3. Build!

csujedihy: Note that I put the hidapi.dll in the project and if you don't believe it's secure, you can build a new one (x64) and replace it.

## Linux
Please go to this repo [https://github.com/velum/lguf-brightness](https://github.com/velum/lguf-brightness).
