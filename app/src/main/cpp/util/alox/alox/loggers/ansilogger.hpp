﻿// #################################################################################################
//  aworx::lox::loggers - ALox Logging Library
//
//  Copyright 2013-2017 A-Worx GmbH, Germany
//  Published under 'Boost Software License' (a free software license, see LICENSE.txt)
// #################################################################################################
/** @file */ // Hello Doxygen

// include ALox main header first...
#if !defined (HPP_ALOX)
    #include "alox/alox.hpp"
#endif

// then, set include guard
#ifndef HPP_ALOX_ANSI_LOGGER
#define HPP_ALOX_ANSI_LOGGER 1


// #################################################################################################
// includes
// #################################################################################################
#if !defined (HPP_ALOX_CORE_TEXTLOGGER_TEXTLOGGER)
    #include "alox/core/textlogger/textlogger.hpp"
#endif

#if !defined (_GLIBCXX_IOSFWD )
    #include <iosfwd>
#endif


namespace aworx { namespace lox { namespace loggers {

/** ************************************************************************************************
 * A logger that logs all messages to the <em>std::basic_ostream</em> instance provided in the
 * constructor. The name of the \e Logger defaults to "ANSI_LOGGER".
 *
 * ALox text logger escape sequences (see class \ref aworx::lox::ESC "ESC")
 * are translated to ANSI escape sequences.
 * Support for ANSI escape sequences (also referred to as <em>VT100 terminal emulation</em>)
 * is available on most unix terminal windows. Besides text colors, bold and italics font style
 * can be set.
 * ANSI escape sequences are also available in various IDE output windows.
 *
 * Foreground and background colors can be set to be either light/dark or dark/light. This improves
 * the readability of log output a lot and even allows to read if foreground and background colors
 * are the same (they then still differ). However, the right setting for this is dependent on
 * the color scheme of the final output device (window). To manipulate the right setting, see field
 * #UseLightColors and also configuration variable
 * [ALOX_CONSOLE_LIGHT_COLORS](../group__GrpALoxConfigVars.html).
 *
 * In the constructor, a default format string and some other definitions in member
 * \ref MetaInfo get set to include color settings.
 * Of-course, these publicly accessible format attributes can be customized after creation.
 *
 * \note Instead of using ANSI sequences in the format strings directly, which would lower the
 * runtime cost a little, ESC sequences are used because this way the light/dark color
 * selection, which depends on the field #UseLightColors is performed correctly.
 *
 * There is not 100% match between the ANSI sequences and the definitions in
 * \ref aworx::lox::ESC "ESC".
 * For example ESC does not provide all ANSI colors and no blinking. On the other hand,
 * ANSI does not allow to reset the style without resetting the colors.
 * Of-course, it is no problem to log other ANSI codes directly into an \b %AnsiLogger.
 * In this case, other Loggers that might be attached to the same Lox and that do not
 * support ANSI must be equipped with corresponding replacement information.
 * In other words: To support the same log output into different loggers, it is
 * recommended to use \ref aworx::lox::ESC "ESC"  sequences instead of
 * directly using ANSI codes.
 *
 * The ANSI codes used by this class are exposed through a list of fields.
 * They might be useful for manipulating the attributes of the \ref MetaInfo
 * member, which of-course might contain native ANSI sequences.
 * (In contrast to the log messages themselves, this meta information is specific to a logger
 * instance and this way it does not need to be replaced in other loggers).
 **************************************************************************************************/
class AnsiLogger : public aworx::lox::core::textlogger::TextLogger
{
    // #############################################################################################
    // ANSI Escape Code definitions
    // #############################################################################################
    public:
    #if defined(_MSC_VER)
    // MSC  (as of 12/2015):
    // C4579: in-class initialization for type 'const aworx::SLiteral<10>'
    // is not yet implemented; static member will remain uninitialized at runtime but
    // use in constant-expressions is supported
    ALIB_API static     SLiteral<5>  ANSI_RED              ; ///< Select red as foreground color
    ALIB_API static     SLiteral<5>  ANSI_GREEN            ; ///< Select green as foreground color
    ALIB_API static     SLiteral<5>  ANSI_YELLOW           ; ///< Select yellow as foreground color
    ALIB_API static     SLiteral<5>  ANSI_BLUE             ; ///< Select blue as foreground color
    ALIB_API static     SLiteral<5>  ANSI_MAGENTA          ; ///< Select magenta as foreground color
    ALIB_API static     SLiteral<5>  ANSI_CYAN             ; ///< Select cyan as foreground color
    ALIB_API static     SLiteral<5>  ANSI_BLACK            ; ///< Select black as foreground color
    ALIB_API static     SLiteral<10> ANSI_WHITE            ; ///< Select white as foreground color
    ALIB_API static     SLiteral<11> ANSI_GRAY             ; ///< Select gray as foreground color
    ALIB_API static     SLiteral<5>  ANSI_STD_COL          ; ///< Select standard foreground color

    ALIB_API static     SLiteral<5>  ANSI_BG_RED           ; ///< Select red as background color
    ALIB_API static     SLiteral<5>  ANSI_BG_GREEN         ; ///< Select green as background color
    ALIB_API static     SLiteral<5>  ANSI_BG_YELLOW        ; ///< Select yellow as background color
    ALIB_API static     SLiteral<5>  ANSI_BG_BLUE          ; ///< Select blue as background color
    ALIB_API static     SLiteral<5>  ANSI_BG_MAGENTA       ; ///< Select magenta as background color
    ALIB_API static     SLiteral<5>  ANSI_BG_CYAN          ; ///< Select cyan as background color
    ALIB_API static     SLiteral<5>  ANSI_BG_BLACK         ; ///< Select black as background color
    ALIB_API static     SLiteral<10> ANSI_BG_WHITE         ; ///< Select white as background color
    ALIB_API static     SLiteral<11> ANSI_BG_GRAY          ; ///< Select gray as background color
    ALIB_API static     SLiteral<5>  ANSI_BG_STD_COL       ; ///< Select standard background color

    ALIB_API static     SLiteral<10> ANSI_LIGHT_RED        ; ///< Select light red as foreground color
    ALIB_API static     SLiteral<10> ANSI_LIGHT_GREEN      ; ///< Select light green as foreground color
    ALIB_API static     SLiteral<10> ANSI_LIGHT_YELLOW     ; ///< Select light yellow as foreground color
    ALIB_API static     SLiteral<10> ANSI_LIGHT_BLUE       ; ///< Select light blue as foreground color
    ALIB_API static     SLiteral<10> ANSI_LIGHT_MAGENTA    ; ///< Select light magenta as foreground color
    ALIB_API static     SLiteral<10> ANSI_LIGHT_CYAN       ; ///< Select light cyan as foreground color
    ALIB_API static     SLiteral<11> ANSI_LIGHT_GRAY       ; ///< Select light gray as foreground color
    ALIB_API static     SLiteral<5>  ANSI_LIGHT_STD_COL    ; ///< Select standard foreground color

    ALIB_API static     SLiteral<10> ANSI_BG_LIGHT_RED     ; ///< Select light red as background color
    ALIB_API static     SLiteral<10> ANSI_BG_LIGHT_GREEN   ; ///< Select light green as background color
    ALIB_API static     SLiteral<10> ANSI_BG_LIGHT_YELLOW  ; ///< Select light yellow as background color
    ALIB_API static     SLiteral<10> ANSI_BG_LIGHT_BLUE    ; ///< Select light blue as background color
    ALIB_API static     SLiteral<10> ANSI_BG_LIGHT_MAGENTA ; ///< Select light magenta as background color
    ALIB_API static     SLiteral<10> ANSI_BG_LIGHT_CYAN    ; ///< Select light cyan as background color
    ALIB_API static     SLiteral<11> ANSI_BG_LIGHT_GRAY    ; ///< Select light gray as background color
    ALIB_API static     SLiteral<5>  ANSI_BG_LIGHT_STD_COL ; ///< Select standard background color

    ALIB_API static     SLiteral<4>  ANSI_BOLD             ; ///< Select bold font style
    ALIB_API static     SLiteral<4>  ANSI_ITALICS          ; ///< Select italics font style
    ALIB_API static     SLiteral<4>  ANSI_STD_STYLE        ; ///< Select standard font style
    ALIB_API static     SLiteral<4>  ANSI_RESET            ; ///< Reset colors and font style

    #else

    static constexpr    SLiteral<5>  ANSI_RED              { "\033[31m"       };
    static constexpr    SLiteral<5>  ANSI_GREEN            { "\033[32m"       };
    static constexpr    SLiteral<5>  ANSI_YELLOW           { "\033[33m"       };
    static constexpr    SLiteral<5>  ANSI_BLUE             { "\033[34m"       };
    static constexpr    SLiteral<5>  ANSI_MAGENTA          { "\033[35m"       };
    static constexpr    SLiteral<5>  ANSI_CYAN             { "\033[36m"       };
    static constexpr    SLiteral<5>  ANSI_BLACK            { "\033[30m"       };
    static constexpr    SLiteral<10> ANSI_WHITE            { "\033[38;5;15m"  };
    static constexpr    SLiteral<11> ANSI_GRAY             { "\033[38;5;240m" };
    static constexpr    SLiteral<5>  ANSI_STD_COL          { "\033[39m"       };

    static constexpr    SLiteral<5>  ANSI_BG_RED           { "\033[41m"       };
    static constexpr    SLiteral<5>  ANSI_BG_GREEN         { "\033[42m"       };
    static constexpr    SLiteral<5>  ANSI_BG_YELLOW        { "\033[43m"       };
    static constexpr    SLiteral<5>  ANSI_BG_BLUE          { "\033[44m"       };
    static constexpr    SLiteral<5>  ANSI_BG_MAGENTA       { "\033[45m"       };
    static constexpr    SLiteral<5>  ANSI_BG_CYAN          { "\033[46m"       };
    static constexpr    SLiteral<5>  ANSI_BG_BLACK         { "\033[40m"       };
    static constexpr    SLiteral<10> ANSI_BG_WHITE         { "\033[48;5;15m"  };
    static constexpr    SLiteral<11> ANSI_BG_GRAY          { "\033[48;5;240m" };
    static constexpr    SLiteral<5>  ANSI_BG_STD_COL       { "\033[49m"       };

    static constexpr    SLiteral<10> ANSI_LIGHT_RED        { "\033[38;5;09m"  };
    static constexpr    SLiteral<10> ANSI_LIGHT_GREEN      { "\033[38;5;10m"  };
    static constexpr    SLiteral<10> ANSI_LIGHT_YELLOW     { "\033[38;5;11m"  };
    static constexpr    SLiteral<10> ANSI_LIGHT_BLUE       { "\033[38;5;12m"  };
    static constexpr    SLiteral<10> ANSI_LIGHT_MAGENTA    { "\033[38;5;13m"  };
    static constexpr    SLiteral<10> ANSI_LIGHT_CYAN       { "\033[38;5;14m"  };
    static constexpr    SLiteral<11> ANSI_LIGHT_GRAY       { "\033[38;5;250m" };
    static constexpr    SLiteral<5>  ANSI_LIGHT_STD_COL    { "\033[39m"       };

    static constexpr    SLiteral<10> ANSI_BG_LIGHT_RED     { "\033[48;5;09m"  };
    static constexpr    SLiteral<10> ANSI_BG_LIGHT_GREEN   { "\033[48;5;10m"  };
    static constexpr    SLiteral<10> ANSI_BG_LIGHT_YELLOW  { "\033[48;5;11m"  };
    static constexpr    SLiteral<10> ANSI_BG_LIGHT_BLUE    { "\033[48;5;12m"  };
    static constexpr    SLiteral<10> ANSI_BG_LIGHT_MAGENTA { "\033[48;5;13m"  };
    static constexpr    SLiteral<10> ANSI_BG_LIGHT_CYAN    { "\033[48;5;14m"  };
    static constexpr    SLiteral<11> ANSI_BG_LIGHT_GRAY    { "\033[48;5;250m" };
    static constexpr    SLiteral<5>  ANSI_BG_LIGHT_STD_COL { "\033[49m"       };

    static constexpr    SLiteral<4>  ANSI_BOLD             { "\033[1m"        };
    static constexpr    SLiteral<4>  ANSI_ITALICS          { "\033[3m"        };
    static constexpr    SLiteral<4>  ANSI_STD_STYLE        { "\033[0m"        };
    static constexpr    SLiteral<4>  ANSI_RESET            { "\033[0m"        };
    #endif



    // #############################################################################################
    // protected fields
    // #############################################################################################
    protected:
        /** The output stream provided in the constructor.               */
        std::basic_ostream<char>*           oStream;

    // #############################################################################################
    // public fields
    // #############################################################################################
    public:
        /** Denotes states of field #UseLightColors.  */
        enum class LightColorUsage
        {
            _Undefined,      ///< Internal, temporary state
            Never,           ///< Never use light colors
            ForegroundLight, ///< Use light colors for foreground
            ForegroundDark   ///< Use light colors for background
        };

        /**
         * Foreground and background colors chosen by this class might differ in their intensity.
         * This increases the overall readability by increasing the contrast.
         * If the background color of a console window is dark, then the background colors of
         * colored log output should be darker colors than the foreground colors - and vice versa.
         *
         * Depending on the setting of this field, ALox
         * \ref aworx::lox::ESC "escape codes" for colors are translated to normal ANSI colors or
         * lighter ones:
         * - If this field is \ref LightColorUsage "LightColorUsage::Never", light colors are
         *   never used.
         * - If this field is \ref LightColorUsage "LightColorUsage::ForegroundLight", foreground
         *   colors will be light colors and background colors dark ones. This is the default.
         * - If \ref LightColorUsage "LightColorUsage::ForegroundDark", background colors will be
         *   light colors and foreground colors dark ones.
         *
         * The configuration variable [ALOX_CONSOLE_LIGHT_COLORS](../group__GrpALoxConfigVars.html)
         * allows to externally modify this flag. It is read once within the constructor .
         */
        LightColorUsage    UseLightColors;

    // #############################################################################################
    // Constructor/destructor
    // #############################################################################################
    public:
        /** ****************************************************************************************
         * Creates an AnsiLogger.
         * @param pOStream       The output stream to write into.
         * @param usesStdStreams Denotes whether this logger writes to the
         *                       <em>standard output streams</em>.
         * @param name           The name of the \e Logger, defaults to what is provided with
         *                       parameter \p typeName.
         * @param typeName       The type of the \e Logger, defaults to "ANSI".
         ******************************************************************************************/
        ALOX_API
        explicit        AnsiLogger( std::basic_ostream<char>* pOStream, bool  usesStdStreams,
                                    const String& name= nullptr, const String& typeName= "ANSI" );

        /** ****************************************************************************************
         * Destructs an AnsiLogger
         ******************************************************************************************/
        ALOX_API
        virtual        ~AnsiLogger();

    // #############################################################################################
    // Interface
    // #############################################################################################
    public:

    // #############################################################################################
    // Abstract method implementations
    // #############################################################################################
    protected:
        /** ****************************************************************************************
         * Implementation of the abstract method of parent class TextLogger.
         * Logs messages to the basic output stream provided in the constructor. Replaces
         * ALox ESC escape sequences with ANSI escape sequences.
         *
         * @param domain      The <em>Log Domain</em>.
         * @param verbosity   The verbosity. This has been checked to be active already on this
         *                    stage and is provided to be able to be logged out only.
         * @param msg         The log message.
         * @param scope       Information about the scope of the <em>Log Statement</em>..
         * @param lineNumber  The line number of a multi-line message, starting with 0. For
         *                    single line messages this is -1.
         ******************************************************************************************/
        ALOX_API
        virtual void logText(  core::Domain&       domain,
                                 Verbosity         verbosity,
                                 AString&          msg,
                                 core::ScopeInfo&  scope,
                                 int               lineNumber);

        /** ****************************************************************************************
         * Empty implementation.
         ******************************************************************************************/
        virtual void notifyMultiLineOp ( lib::lang::Phase ) {}



}; // class AnsiLogger

/** ************************************************************************************************
 * A #AnsiLogger that logs all messages to the standard output <em>cout</em>.
 * The name of the \e Logger defaults to "ANSI_CONSOLE".
 *
 * Provides 'cout' to the constructor of its parent class %AnsiLogger.
 * See class #AnsiLogger for more information on ANSI escape sequences and their use.
 *
 * \note To avoid misunderstandings: This class can not enable the output console (which receives
 *       ALox log data) to support ANSI Escape Codes. The opposite is right: this class should be
 *       used only if the console supports ANSI Escape Codes.
 **************************************************************************************************/
class AnsiConsoleLogger : public AnsiLogger
{
    // #############################################################################################
    // Constructor/destructor
    // #############################################################################################
    public:
        /** ****************************************************************************************
         * Creates an AnsiConsoleLogger.
         * @param name     (Optional) The name of the \e Logger, defaults to "CONSOLE".
         ******************************************************************************************/
        ALOX_API
        explicit            AnsiConsoleLogger( const String& name= nullptr  );

        /** ****************************************************************************************
         * Destructs an AnsiConsoleLogger
         ******************************************************************************************/
        ALOX_API
        virtual            ~AnsiConsoleLogger();
}; // class AnsiConsoleLogger


}} // namespace lox::loggers

/** Type alias name in namespace #aworx. */
using     AnsiLogger=           aworx::lox::loggers::AnsiLogger;

/** Type alias name in namespace #aworx. */
using     AnsiConsoleLogger=    aworx::lox::loggers::AnsiConsoleLogger;

}  // namespace aworx

#endif // HPP_ALOX_ANSI_LOGGER

