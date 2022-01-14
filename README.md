# SCUMM Translations to PO format

## Description

Small tool to translate SCUMM games into other languages using POT/PO file format (poedit,...)
Dependencies: You need the thrid-party tool "scummtr.exe", which has been included here in third party folder.

## Workflow

     _____________    scummtr.exe      _______________   scummtr2po -e   ________________________
    |             |-----------------> | strings.txt   | --------------->|  game.pot (base file)  | ----> open in PoEdit/Weblate/...
    |             |                   '_______________'                 |  strings.txt.cfg       |             |
    | SCUMM GAME  |                    _______________                  '________________________'       Create translations (ca, fr,..)
    |             |<------------------| stringsCa.txt |                  _____________________________         |
    '_____________'   scummtr.exe     '_______________' <---------------|  ca.po (translation to ca)  |<-------'
                                                         scummtr2po -i  '_____________________________' 

* game.pot is the base file for the PO translations.
* strings.txt.cfg  is an internal config file for scummtr2po
* ca.po is the translated file to Catalan


## Example

### export strings to open in POEDIT

Example with Indiana Jones and the fate of Atlantis:

    scummtr.exe -g atlantis -p AtlantisEN -h -I -o -f strings.txt
    scummtr2po.exe --export --strings strings.txt --po atlantis.pot

You can open atlantis.pot in Poedit and create translations for some language (i.e. Catalan). Poedit will create file ca.po and as many as languages translated (i.e. fr.po, en.po,..)

### import translations from POEDIT to SCUMM

    scummtr2po.exe --import --po ca.po --strings stringsCa.txt [ --config strings.txt.cfg ]
    scummtr.exe -g atlantis -p AtlantisEN -h -I -i -f stringsCa.txt



## Thank you

Scummtr.exe was made by Thomas Combeleran but I could not find the original source.
I obtained it through Laurence Dougal website (great tools for Scumm!): http://www.jestarjokin.net/apps/scummspeaks/
Where he links it here: http://hibernatus34.free.fr/scumm/scummtr.exe

