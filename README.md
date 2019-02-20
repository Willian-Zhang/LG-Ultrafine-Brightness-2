# LG-Ultrafine-Brightness 2

A tool to adjust brightness of LG Ultrafine 4k/5K **silently** on **Windows** without the help of Bootcamp.

The program simply send set feature report to the device via USB and then the LG 4/5K monitor adjust the brightness upon receiving such reports.

Thanks to @[csujedihy](https://github.com/csujedihy/), @[unknownzerx](https://github.com/unknownzerx/),
This project is derived from [csujedihy/LG-Ultrafine-Brightness](https://github.com/csujedihy/LG-Ultrafine-Brightness) which is also derived from [unknownzerx/lguf-brightness](https://github.com/unknownzerx/lguf-brightness).

## Download
[Link to zip](https://github.com/Willian-Zhang/LG-Ultrafine-Brightness-2/releases/download/v2.5.1/brightness-2-5-1.zip)

## Features
- **Adjust brightness silently** in background, thus Apps runing in forground won't be interupted
- Use _shortcut_ or _filename_ as **option** to activate brightness adjustment
- Redesigned in memory and **speed** conservative manner

## Usage
Just double click.

### Sample usages
#### Show brightness
1. Unzip and copy the files into `C://Program Files (x86)/LGBrightness/`
1. Right click `LG-Ultrafine-Brightness.exe` > Send to > Desktop (create shortcut)
1. Right click `LG-Ultrafine-Brightness.exe shortcut` on Desktop > Properties > `Target:` > add `␣p` (space then `p`) at the very end.

Now brighness would showup upon open the desktop shortcut.

#### Change to maximium brightness upon runnning
Same as in `Show brightness` execpt instead of add `p` to the end, add `9` to the end.

#### Use Crtl(control) + F2 to brightness up in game
- Install [3RVX](https://3rvx.com/)(a volume among other controls with keyboard shortcut utility) first
1. Unzip and copy the files into `C://Program Files (x86)/LGBrightness/`
1. Duplicate `LG-Ultrafine-Brightness.exe` and rename the new one as `LG-Ultrafine-Brightness.+.exe`
1. Test if brightness goes up when open up `LG-Ultrafine-Brightness.+.exe`
1. Modify 3RVX Settings
	1. Right click 3RVX > settings
	1. Click `Hotkeys` Tab
	1. Click `+` to add a new Action
	1. Click Hotkey, then press `Ctrl + F2` to register that as the new hotkey
	1. Select `Run` in `Action`
	1. Select the `LG-Ultrafine-Brightness.+.exe` 
	
_You may also want to create one for `brightness down`, use `LG-Ultrafine-Brightness.-.exe` and `Ctrl + F1` or whatever hotkey you want_

Press the `Crtl(control) + F2` you set, brightness changes! 🎉
 
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
        - See `Sample usages`
- Or any other hotkey applications with `Run...` capabilities


## History 
- 2.5.1
	- fix help doc
- 2.5:
	- Rearchitected 
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
