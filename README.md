# Shruthi-Editor:

An unofficial Editor for the Shruthi hardware synthesizer. For informations
about the Shruthi, see <http://mutable-instruments.net/shruthi1>.

Application icon: "saraswati" by Francois Barbut.

Uses RtMidi 2.1.0 by Gary P. Scavone <http://music.mcgill.ca/~gary/rtmidi/>.

The newest version can be found at <https://github.com/Manuel-K/shruthi-editor>.


## Using the editor:
Make sure to select the midi device, midi channel and the filter board of the 
Shruthi in the settings dialog.

The editor has bidirectional communication: Changing a parameter in the editor 
changes the same parameter on the Shruthi instantly and vice versa.

Patches/sequences can be read from mixed SysEx files. The editor searches the 
first valid patch/sequence from the beginning of the file.

After loading a patch you have to send the patch manually by using the
"Shruthi">"Send Patch" menu option.

# Notes on patch versions:
The version of the current patch is displayed at the right side of the status 
bar. It shows "1.xx" for Shruthi firmware versions greater than 1.00 and "0.9x"
for versions before 1.00. You can switch modes by using the two different reset
patch entries in the menu, by loading a patch from a file or by receiving a 
patch from the Shruthi.

If you use a Shruthi firmware version before 1.00, you have to use the 
"File"->"Reset Patch (pre 1.00)" menu entry, or by loading/receiving an old 
version patch. Converting an new version patch to the old version is not 
supported. 

If you use an up-to-date Shruthi firmware and want to convert an old patch,
you have to send it to the Shruthi and fetch it back by clicking 
"Shruthi"->"Send Patch" and "Shruthi"->"Fetch Patch".

Using "File"->"Randomize Patch" will not change the patch version.



## What it can do:
 * Editing patches.
 * Remotely changing patch options on the Shruthi.
 * Loading/saving patches/sequences from/to disk as SysEx.
 * Fetching/sending patches/sequences from/to the Shruthi.
 * Changes made to patch options on the Shruthi change the settings of the
   editor.
 * Generate random patches.
 * Editing patches/sequences up to (and including) firmware version 1.02.

## Known issues:
 * Receiving LFO 1/2 rates per CC (used by firmware version 1.01 and 1.02) is
   not optimal, but it should map to the Shruthi's values.
 * Filseclab (at least versions from Jun 2015) flags the Windows binary compiled 
   by gcc as Packed.NSAnti.b.fomm.mg; the 43 other scan engines used by Metascan 
   don't detect anything. This is called a **false positive**. The Windows 
   binaries provided on github (starting with v0.23) are compiled on Linux using 
   MXE and are uploaded on Linux.

## Compiling the source code:

### Prerequisites:
You need Qt 5 (or Qt 4) <http://www.qt.io/download-open-source/>.
If you want to compile the editor on windows, choose the mingw version.


### Compiling:
Run 

    qmake -config release
    make

inside the shruthi-editor source folder.


### Note:
If you want to use firmware versions prior to 0.94, you have to open
shruthi-editor.pro and change the line

    #DEFINES += PRE094SYSEXHEADER

to

    DEFINES += PRE094SYSEXHEADER
 


## Changes:
* v0.16
  * support for dual VCF -> new ui layout
  * one minor bugfix
  * old ui layouts are currently broken (e.g. shruthi-editor-onepage.ui,
    shruthi-editor-tabs.ui)
* v0.17
  * change from portmidi to RtMidi
  * made save format cross system compatible; this breaks backwards
   compatibility
  * added keyboard with panic button
* v0.18
  * refactored parts of the code
  * added status bar messages to most actions
  * added simple check if patch file is valid
  * added ability to load/save patches in sysex format 
* v0.19
  * tweaked the keyboard dialog. keys are now black/white
  * updated to RtMidi 2.1.0
  * updated to Qt 5
* v0.20 skipped
* v0.21
  * SysEx header for firmware versions >= 0.94
  * updated patch definitions
  * set midi client/port names (no more default RtMidi names)
  * detect patch version (pre 1.00, post 1.00)
  * detect Shruthi firmware version (pre 0.98, 0.98 and post 1.00)
  * process CC (needed for firmwares >= 1.00)
  * added missing labels
  * fixed crash during attempt to change unknown parameters
  * try to emulate Shruthi's LFO rate mapping
  * support for all filter boards (firmware version 1.02)
  * added debouncing
  * updated init patch (old version can be reached via menu)
  * added patch version display to statusbar
* v0.22
  * scalable keyboad with flexible amount of keys
  * added sequence editor
  * support for loading of mixed sysex files: load patch/sequence searches 
    the first occurence in a sysex file
  * added ui for missing patch parameters
  * send parameter changes as cc (compile time decision)
* v0.23
  * combined patch/sequence load and save dialogs (with type selection)
  * combined patch/sequence send and fetch menu action
  * added program library
  * minor cosmetic changes
* v0.24
  * fixed patch/sequence number in status bar
  * fixed div by zero in fetchProgress/sendProgress
  * disallowed selection of multiple ranges
  * non-blocking library send function
  * re-added ability to compile with qt 4
  * fixed crash due to RtMidi not initializing
  * warn about RtMidi not initilizing in settings dialog
  * hide editor instances during port selection
