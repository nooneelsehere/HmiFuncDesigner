
include(../../HmiFuncDesignerLibrary.pri)

!win32:VERSION = 13.0.0

TEMPLATE = lib
CONFIG += qt warn_off thread exceptions hide_symbols

CONFIG(debug, debug|release) {
    mac: {
        TARGET = qscintilla2_qt$${QT_MAJOR_VERSION}_debug
    } else {
        win32: {
            TARGET = qscintilla2_qt$${QT_MAJOR_VERSION}d
        } else {
            TARGET = qscintilla2_qt$${QT_MAJOR_VERSION}
        }
    }
} else {
    TARGET = qscintilla2_qt$${QT_MAJOR_VERSION}
}

QTSCINTILLA_LIBDIR = $$IDE_LIBRARY_PATH
#$$OUT_PWD/../../../lib
#message(pwd: $$OUT_PWD)
DESTDIR = $$QTSCINTILLA_LIBDIR

INSTALL_LIBRARY_PATH = $$IDE_BIN_PATH
#$$OUT_PWD/../../exec/bin

target.path = $$INSTALL_LIBRARY_PATH
INSTALLS += target

!macx {
    win32 {
        dlltarget.path = $$INSTALL_LIBRARY_PATH
        INSTALLS += dlltarget
    } else {
        target.path = $$INSTALL_LIBRARY_PATH
        INSTALLS += target
    }
}

INCLUDEPATH += . ./include ./lexlib ./src

!CONFIG(staticlib) {
    DEFINES += QSCINTILLA_MAKE_DLL
}
DEFINES += SCINTILLA_QT SCI_LEXER

greaterThan(QT_MAJOR_VERSION, 4) {
	QT += widgets printsupport

    greaterThan(QT_MINOR_VERSION, 1) {
	    macx:QT += macextras
    }

    # Work around QTBUG-39300.
    CONFIG -= android_install
}

# For old versions of GCC.
unix:!macx {
    CONFIG += c++11
}

# Comment this in if you want the internal Scintilla classes to be placed in a
# Scintilla namespace rather than pollute the global namespace.
#DEFINES += SCI_NAMESPACE

####################>>########################
# removed by JasonWang

# target.path = $$[QT_INSTALL_LIBS]
# INSTALLS += target

# header.path = $$[QT_INSTALL_HEADERS]
# header.files = Qsci
# INSTALLS += header

# trans.path = $$[QT_INSTALL_TRANSLATIONS]
# trans.files = qscintilla_*.qm
# INSTALLS += trans

# qsci.path = $$[QT_INSTALL_DATA]
# qsci.files = ./qsci
# INSTALLS += qsci

####################<<########################

greaterThan(QT_MAJOR_VERSION, 4) {
    features.path = $$[QT_HOST_DATA]/mkspecs/features
} else {
    features.path = $$[QT_INSTALL_DATA]/mkspecs/features
}
CONFIG(staticlib) {
    features.files = $$PWD/features_staticlib/qscintilla2.prf
} else {
    features.files = $$PWD/features/qscintilla2.prf
}

# removed by JasonWang
# INSTALLS += features

HEADERS = \
	./Qsci/qsciglobal.h \
	./Qsci/qsciscintilla.h \
	./Qsci/qsciscintillabase.h \
	./Qsci/qsciabstractapis.h \
	./Qsci/qsciapis.h \
	./Qsci/qscicommand.h \
	./Qsci/qscicommandset.h \
	./Qsci/qscidocument.h \
	./Qsci/qscilexer.h \
	./Qsci/qscilexeravs.h \
	./Qsci/qscilexerbash.h \
	./Qsci/qscilexerbatch.h \
	./Qsci/qscilexercmake.h \
	./Qsci/qscilexercoffeescript.h \
	./Qsci/qscilexercpp.h \
	./Qsci/qscilexercsharp.h \
	./Qsci/qscilexercss.h \
	./Qsci/qscilexercustom.h \
	./Qsci/qscilexerd.h \
	./Qsci/qscilexerdiff.h \
	./Qsci/qscilexerfortran.h \
	./Qsci/qscilexerfortran77.h \
	./Qsci/qscilexerhtml.h \
	./Qsci/qscilexeridl.h \
	./Qsci/qscilexerjava.h \
	./Qsci/qscilexerjavascript.h \
	./Qsci/qscilexerjson.h \
	./Qsci/qscilexerlua.h \
	./Qsci/qscilexermakefile.h \
	./Qsci/qscilexermarkdown.h \
	./Qsci/qscilexermatlab.h \
	./Qsci/qscilexeroctave.h \
	./Qsci/qscilexerpascal.h \
	./Qsci/qscilexerperl.h \
	./Qsci/qscilexerpostscript.h \
	./Qsci/qscilexerpo.h \
	./Qsci/qscilexerpov.h \
	./Qsci/qscilexerproperties.h \
	./Qsci/qscilexerpython.h \
	./Qsci/qscilexerruby.h \
	./Qsci/qscilexerspice.h \
	./Qsci/qscilexersql.h \
	./Qsci/qscilexertcl.h \
	./Qsci/qscilexertex.h \
	./Qsci/qscilexerverilog.h \
	./Qsci/qscilexervhdl.h \
	./Qsci/qscilexerxml.h \
	./Qsci/qscilexeryaml.h \
	./Qsci/qscimacro.h \
	./Qsci/qsciprinter.h \
	./Qsci/qscistyle.h \
	./Qsci/qscistyledtext.h \
	ListBoxQt.h \
	SciClasses.h \
	SciNamespace.h \
	ScintillaQt.h \
        ./include/ILexer.h \
        ./include/Platform.h \
        ./include/Sci_Position.h \
        ./include/SciLexer.h \
        ./include/Scintilla.h \
        ./include/ScintillaWidget.h \
        ./lexlib/Accessor.h \
        ./lexlib/CharacterCategory.h \
        ./lexlib/CharacterSet.h \
        ./lexlib/LexAccessor.h \
        ./lexlib/LexerBase.h \
        ./lexlib/LexerModule.h \
        ./lexlib/LexerNoExceptions.h \
        ./lexlib/LexerSimple.h \
        ./lexlib/OptionSet.h \
        ./lexlib/PropSetSimple.h \
        ./lexlib/StringCopy.h \
        ./lexlib/StyleContext.h \
        ./lexlib/SubStyles.h \
        ./lexlib/WordList.h \
        ./src/AutoComplete.h \
        ./src/CallTip.h \
        ./src/CaseConvert.h \
        ./src/CaseFolder.h \
        ./src/Catalogue.h \
        ./src/CellBuffer.h \
        ./src/CharClassify.h \
        ./src/ContractionState.h \
        ./src/Decoration.h \
        ./src/Document.h \
        ./src/EditModel.h \
        ./src/Editor.h \
        ./src/EditView.h \
        ./src/ExternalLexer.h \
        ./src/FontQuality.h \
        ./src/Indicator.h \
        ./src/KeyMap.h \
        ./src/LineMarker.h \
        ./src/MarginView.h \
        ./src/Partitioning.h \
        ./src/PerLine.h \
        ./src/PositionCache.h \
        ./src/RESearch.h \
        ./src/RunStyles.h \
        ./src/ScintillaBase.h \
        ./src/Selection.h \
        ./src/SplitVector.h \
        ./src/Style.h \
        ./src/UnicodeFromUTF8.h \
        ./src/UniConversion.h \
        ./src/ViewStyle.h \
        ./src/XPM.h

SOURCES = \
	qsciscintilla.cpp \
	qsciscintillabase.cpp \
	qsciabstractapis.cpp \
	qsciapis.cpp \
	qscicommand.cpp \
	qscicommandset.cpp \
	qscidocument.cpp \
	qscilexer.cpp \
	qscilexeravs.cpp \
	qscilexerbash.cpp \
	qscilexerbatch.cpp \
	qscilexercmake.cpp \
	qscilexercoffeescript.cpp \
	qscilexercpp.cpp \
	qscilexercsharp.cpp \
	qscilexercss.cpp \
	qscilexercustom.cpp \
	qscilexerd.cpp \
	qscilexerdiff.cpp \
	qscilexerfortran.cpp \
	qscilexerfortran77.cpp \
	qscilexerhtml.cpp \
	qscilexeridl.cpp \
	qscilexerjava.cpp \
	qscilexerjavascript.cpp \
	qscilexerjson.cpp \
	qscilexerlua.cpp \
	qscilexermakefile.cpp \
	qscilexermarkdown.cpp \
	qscilexermatlab.cpp \
	qscilexeroctave.cpp \
	qscilexerpascal.cpp \
	qscilexerperl.cpp \
	qscilexerpostscript.cpp \
	qscilexerpo.cpp \
	qscilexerpov.cpp \
	qscilexerproperties.cpp \
	qscilexerpython.cpp \
	qscilexerruby.cpp \
	qscilexerspice.cpp \
	qscilexersql.cpp \
	qscilexertcl.cpp \
	qscilexertex.cpp \
	qscilexerverilog.cpp \
	qscilexervhdl.cpp \
	qscilexerxml.cpp \
	qscilexeryaml.cpp \
	qscimacro.cpp \
	qsciprinter.cpp \
	qscistyle.cpp \
	qscistyledtext.cpp \
        MacPasteboardMime.cpp \
        InputMethod.cpp \
	SciClasses.cpp \
	ListBoxQt.cpp \
	PlatQt.cpp \
	ScintillaQt.cpp \
        ./lexers/LexA68k.cpp \
        ./lexers/LexAbaqus.cpp \
        ./lexers/LexAda.cpp \
        ./lexers/LexAPDL.cpp \
        ./lexers/LexAsm.cpp \
        ./lexers/LexAsn1.cpp \
        ./lexers/LexASY.cpp \
        ./lexers/LexAU3.cpp \
        ./lexers/LexAVE.cpp \
        ./lexers/LexAVS.cpp \
        ./lexers/LexBaan.cpp \
        ./lexers/LexBash.cpp \
        ./lexers/LexBasic.cpp \
        ./lexers/LexBatch.cpp \
        ./lexers/LexBibTex.cpp \
        ./lexers/LexBullant.cpp \
        ./lexers/LexCaml.cpp \
        ./lexers/LexCLW.cpp \
        ./lexers/LexCmake.cpp \
        ./lexers/LexCOBOL.cpp \
        ./lexers/LexCoffeeScript.cpp \
        ./lexers/LexConf.cpp \
        ./lexers/LexCPP.cpp \
        ./lexers/LexCrontab.cpp \
        ./lexers/LexCsound.cpp \
        ./lexers/LexCSS.cpp \
        ./lexers/LexD.cpp \
        ./lexers/LexDiff.cpp \
        ./lexers/LexDMAP.cpp \
        ./lexers/LexDMIS.cpp \
        ./lexers/LexECL.cpp \
        ./lexers/LexEDIFACT.cpp \
        ./lexers/LexEiffel.cpp \
        ./lexers/LexErlang.cpp \
        ./lexers/LexErrorList.cpp \
        ./lexers/LexEScript.cpp \
        ./lexers/LexFlagship.cpp \
        ./lexers/LexForth.cpp \
        ./lexers/LexFortran.cpp \
        ./lexers/LexGAP.cpp \
        ./lexers/LexGui4Cli.cpp \
        ./lexers/LexHaskell.cpp \
        ./lexers/LexHex.cpp \
        ./lexers/LexHTML.cpp \
        ./lexers/LexInno.cpp \
        ./lexers/LexJSON.cpp \
        ./lexers/LexKix.cpp \
        ./lexers/LexKVIrc.cpp \
        ./lexers/LexLaTex.cpp \
        ./lexers/LexLisp.cpp \
        ./lexers/LexLout.cpp \
        ./lexers/LexLua.cpp \
        ./lexers/LexMagik.cpp \
        ./lexers/LexMake.cpp \
        ./lexers/LexMarkdown.cpp \
        ./lexers/LexMatlab.cpp \
        ./lexers/LexMetapost.cpp \
        ./lexers/LexMMIXAL.cpp \
        ./lexers/LexModula.cpp \
        ./lexers/LexMPT.cpp \
        ./lexers/LexMSSQL.cpp \
        ./lexers/LexMySQL.cpp \
        ./lexers/LexNimrod.cpp \
        ./lexers/LexNsis.cpp \
        ./lexers/LexNull.cpp \
        ./lexers/LexOpal.cpp \
        ./lexers/LexOScript.cpp \
        ./lexers/LexPascal.cpp \
        ./lexers/LexPB.cpp \
        ./lexers/LexPerl.cpp \
        ./lexers/LexPLM.cpp \
        ./lexers/LexPO.cpp \
        ./lexers/LexPOV.cpp \
        ./lexers/LexPowerPro.cpp \
        ./lexers/LexPowerShell.cpp \
        ./lexers/LexProgress.cpp \
        ./lexers/LexProps.cpp \
        ./lexers/LexPS.cpp \
        ./lexers/LexPython.cpp \
        ./lexers/LexR.cpp \
        ./lexers/LexRebol.cpp \
        ./lexers/LexRegistry.cpp \
        ./lexers/LexRuby.cpp \
        ./lexers/LexRust.cpp \
        ./lexers/LexScriptol.cpp \
        ./lexers/LexSmalltalk.cpp \
        ./lexers/LexSML.cpp \
        ./lexers/LexSorcus.cpp \
        ./lexers/LexSpecman.cpp \
        ./lexers/LexSpice.cpp \
        ./lexers/LexSQL.cpp \
        ./lexers/LexSTTXT.cpp \
        ./lexers/LexTACL.cpp \
        ./lexers/LexTADS3.cpp \
        ./lexers/LexTAL.cpp \
        ./lexers/LexTCL.cpp \
        ./lexers/LexTCMD.cpp \
        ./lexers/LexTeX.cpp \
        ./lexers/LexTxt2tags.cpp \
        ./lexers/LexVB.cpp \
        ./lexers/LexVerilog.cpp \
        ./lexers/LexVHDL.cpp \
        ./lexers/LexVisualProlog.cpp \
        ./lexers/LexYAML.cpp \
        ./lexlib/Accessor.cpp \
        ./lexlib/CharacterCategory.cpp \
        ./lexlib/CharacterSet.cpp \
        ./lexlib/LexerBase.cpp \
        ./lexlib/LexerModule.cpp \
        ./lexlib/LexerNoExceptions.cpp \
        ./lexlib/LexerSimple.cpp \
        ./lexlib/PropSetSimple.cpp \
        ./lexlib/StyleContext.cpp \
        ./lexlib/WordList.cpp \
        ./src/AutoComplete.cpp \
        ./src/CallTip.cpp \
        ./src/CaseConvert.cpp \
        ./src/CaseFolder.cpp \
        ./src/Catalogue.cpp \
        ./src/CellBuffer.cpp \
        ./src/CharClassify.cpp \
        ./src/ContractionState.cpp \
        ./src/Decoration.cpp \
        ./src/Document.cpp \
        ./src/EditModel.cpp \
        ./src/Editor.cpp \
        ./src/EditView.cpp \
        ./src/ExternalLexer.cpp \
        ./src/Indicator.cpp \
        ./src/KeyMap.cpp \
        ./src/LineMarker.cpp \
        ./src/MarginView.cpp \
        ./src/PerLine.cpp \
        ./src/PositionCache.cpp \
        ./src/RESearch.cpp \
        ./src/RunStyles.cpp \
        ./src/ScintillaBase.cpp \
        ./src/Selection.cpp \
        ./src/Style.cpp \
        ./src/UniConversion.cpp \
        ./src/ViewStyle.cpp \
        ./src/XPM.cpp \

TRANSLATIONS = \
	qscintilla_cs.ts \
	qscintilla_de.ts \
	qscintilla_es.ts \
	qscintilla_fr.ts \
	qscintilla_pt_br.ts
