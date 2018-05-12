# How to reverse engineer the iTunes database format (`.itl`)

## Direct file structure analysis

Once the database is decrypted, its structure is pretty straightforward because of the abundant use of ASCII strings marking the start of each blocks. This is the method I've use for guessing most of iTunes library's structure.

## Comparison between decrypted file structure and exported XML files

In order to support integration of iTunes library in third-party software such as Serrato or Traktor, Apple provides an XML/Plist version of the iTunes library. This XML version provides us valuable information about the data available in the database, and Apple's terminology.

However the XML version don't hold all the data avaiable in the proprietary `.itl` version (such as "checked" track info).

## iTunes executable analysis

I did not try this approach, because my country's juridiction is quite vague on that matter. **Please don't do it if you plan on contributing to this repository !**
