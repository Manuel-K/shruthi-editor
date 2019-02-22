greaterThan(QT_MAJOR_VERSION, 5) {
  CONFIG += lrelease
} else: equals(QT_MAJOR_VERSION, 5):!lessThan(QT_MINOR_VERSION, 12) {
  CONFIG += lrelease
} else {
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
    QMAKE_CLEAN         += $${qmfile}
  }
}
