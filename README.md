# 🎸 PinkGuitarFX
 
![JUCE](https://img.shields.io/badge/JUCE-7.x-ff69b4?style=flat-square) ![C++](https://img.shields.io/badge/C++-17-ff69b4?style=flat-square) ![VST3](https://img.shields.io/badge/VST3-AU-ff69b4?style=flat-square)
 
---
 
## 🇷🇺 RU
 
Гитарный дисторшн-плагин на JUCE
 
### 🎛️ Параметры
 
| Параметр | Описание |
|----------|----------|
| 🔥 **DRIVE** | Дисторшн, асимметричный клиппер |
| 🔇 **GATE** | Шумовой гейт, −80...−20 дБ |
| 🔉 **BASS / TREB** | Полочные фильтры ±12 дБ |
| 🌐 **WIDE** | Стерео расширитель — выше 0.7 звучит странно в моно |
| 🔊 **MASTER** | Выходной уровень |
| 📦 **CAB** | Три слота под кабинетные IR + режим OFF |
| 🎵 **TUNER** | Хроматический тюнер, показывает ноту |
 
Оверсэмплинг 4x.
 
### 🔧 Сборка
 
Нужен JUCE 7+. Открываешь `.jucer` в Projucer, экспортируешь, собираешь.  
IR-файлы кладёшь как `lol.wav`, `kek.wav`, `mek.wav` в BinaryData.
 
```bash
git clone https://github.com/safoniy2012/PinkGuitarFX
cd PinkGuitarFX
# открой PinkGuitarFX.jucer в Projucer
```
 
### 📋 Зависимости
 
- JUCE 7.x
- C++17
- macOS 10.15+ / Windows 10+
### ⚠️ Известные проблемы
 
- При смене кабинета на ходу щёлкает
- Тюнер врёт на грязном сигнале — лучше настраиваться до дисторшна
---
 
## 🇬🇧 EN
 
Guitar distortion plugin. Made for personal use.
 
### 🎛️ Controls
 
| Parameter | Description |
|-----------|-------------|
| 🔥 **DRIVE** | Distortion, asymmetric exponential clipper |
| 🔇 **GATE** | Noise gate, −80...−20 dB |
| 🔉 **BASS / TREB** | Shelf filters ±12 dB |
| 🌐 **WIDE** | Stereo widener — gets weird in mono above 0.7 |
| 🔊 **MASTER** | Output gain |
| 📦 **CAB** | Three cabinet IR slots + OFF mode |
| 🎵 **TUNER** | Chromatic tuner |
 
4x oversampling.
 
### 🔧 Building
 
Requires JUCE 7+. Open `.jucer` in Projucer, export, build.  
Place IR files as `lol.wav`, `kek.wav`, `mek.wav` in BinaryData.
 
```bash
git clone https://github.com/safoniy2012/PinkGuitarFX
cd PinkGuitarFX
# open PinkGuitarFX.jucer in Projucer
```
 
### 📋 Dependencies
 
- JUCE 7.x
- C++17
- macOS 10.15+ / Windows 10+
### ⚠️ Known Issues
 
- Clicking when switching IRs during playback
- Tuner is unreliable on distorted signal — tune before the drive stage
