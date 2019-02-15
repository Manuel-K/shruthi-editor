equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 12) {
        message(using a workaround for missing 'lrelease' option in Qt <5.12...)

        for(tsfile, TRANSLATIONS) {
          qmfile   = $$tsfile
          qmfile  ~= s/.ts$/.qm/
          qmfile  ~= s,^translations/,.qm/,

                  thisqmcom  = $${qmfile}.commands
          win32:$$thisqmcom  = md .qm;
           else:$$thisqmcom  = mkdir -p .qm;
                $$thisqmcom += lrelease -qm $$qmfile $$tsfile

            thisqmdep  = $${qmfile}.depends
          $$thisqmdep  = $${tsfile}

          PRE_TARGETDEPS      += $${qmfile}
          QMAKE_EXTRA_TARGETS += $${qmfile}


            thisinst    = translations_$${qmfile}
            thisinstdep = $${thisinst}.depends
          $$thisinstdep = $$qmfile

            thisinstcfg = $${thisinst}.CONFIG
          $$thisinstcfg = no_check_exist

            thisinstfil = $${thisinst}.files
          $$thisinstfil = $$PWD/$$qmfile

            thisinstpat = $${thisinst}.path
          $$thisinstpat = $$QM_FILES_INSTALL_PATH

          INSTALLS += $$thisinst
        }
    }
    else {
        CONFIG += lrelease
    }
}
