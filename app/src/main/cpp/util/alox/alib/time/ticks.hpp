#// #################################################################################################
//  ALib - A-Worx Utility Library
//
//  Copyright 2013-2017 A-Worx GmbH, Germany
//  Published under 'Boost Software License' (a free software license, see LICENSE.txt)
// #################################################################################################
/** @file */ // Hello Doxygen

// check for alib.hpp already there but not us
#if !defined (HPP_ALIB)
    #error "include \"alib/alib.hpp\" before including this header"
#endif
#if defined(HPP_COM_ALIB_TEST_INCLUDES) && defined(HPP_ALIB_TIME_TICKS)
    #error "Header already included"
#endif

// then, set include guard
#ifndef HPP_ALIB_TIME_TICKS
//! @cond NO_DOX
#define HPP_ALIB_TIME_TICKS 1
//! @endcond NO_DOX



// #################################################################################################
// includes
// #################################################################################################
#if !defined (HPP_ALIB_STRINGS_SUBSTRING)
    #include "alib/strings/substring.hpp"
#endif
#if !defined (_GLIBCXX_CTIME) && !defined(_CTIME_)
    #include <ctime>
#endif

namespace aworx { namespace lib {
/**
 * This namespace of ALib holds classes that are providing methods for
 * measuring time spans and conversion of dates and time values.
 */
namespace time {


/** 64-bit integer value used to store raw tick values.  */
using   rawtime =   int64_t;

// #############################################################################################
// Namespace initialization
// #############################################################################################

/**
 * Initializes ALib time  logic.
 * This method should not be called directly. use
 * \ref aworx::lib::ALIB::Init "ALIB::Init" to initialize all <b>%ALib Modules</b> included in the
 * distribution.
 */
ALIB_API void Init();


/**
 * Frees resources and shuts down ALib time logic.
 * This method should not be called directly. use
 * \ref aworx::lib::ALIB::TerminationCleanUp "ALIB::TerminationCleanUp" to de-initialize all
 * <b>%ALib Modules</b> included in the distribution.
 */
ALIB_API void TerminationCleanUp();

/**
 * This method needs to be invoked when values of type \ref Ticks are to be converted to "real"
 * clock time values. Whenever the system clock is changed, such change is reflected by class
 * \b %Ticks and its conversion methods
 * \ref aworx::lib::time::Ticks::InEpochSeconds      "Ticks::InEpochSeconds",
 * \ref aworx::lib::time::Ticks::SetFromEpochSeconds "Ticks::SetFromEpochSeconds",
 * \ref aworx::lib::time::Ticks::SetFromEpochSeconds "Ticks::InSystemTime",
 * \ref aworx::lib::time::Ticks::SetFromEpochSeconds "Ticks::SetFromSystemTime",
 * \ref aworx::lib::time::Ticks::SetFromEpochSeconds "Ticks::InFileTime" and
 * \ref aworx::lib::time::Ticks::SetFromEpochSeconds "Ticks::SetFromFileTime", only after the
 * invocation of this method.<br>
 * The advantages of this approach are:
 * - As long as this method is not called, changes of the system clock do not affect the display
 *   of time values measured with class \b %Ticks. They will continue to display clock time values
 *   which are in alignment with the system clock setting as it was when this method was invoked
 *   last (e.g. the bootstrap of the process).
 * - Round-trip calculations from \b %Ticks to clock times and back are consistent when this method
 *   is not invoked.
 *
 * Of-course, the drawback is: to ensure that the actual clock setting is reflected, the method
 * has to be invoked 'manually' prior to using one of the methods listed above.
 *
 * The method is invoked by \ref Init.
 */
ALIB_API void SyncClock();

/** ************************************************************************************************
 * This class provides an interface into system dependent timer values (usually 64 bit
 * tick counters) that we call *ticks*. Ticks and this class allow to store and calculate time
 * values in an efficient and highly accurate way.
 *
 * Ticks generated by this class can be added and subtracted:
 *
 * - Subtraction generates a *span value in ticks*
 * - *Span value in ticks* can be added to themselves safely
 * - *Span value in ticks* can be added to ticks safely
 *
 * Conversion functions to milliseconds, microseconds and nanoseconds accept ticks and span
 * values in ticks in the same manner.
 *
 * As Ticks are system dependent, they should not be stored and loaded in data files or
 * otherwise shared between systems. Even on the same machine, the values may become be
 * incompatible (e.g. after a reboot).
 * Also, in general such values should only be used by methods of
 * this class and classes
 * \ref aworx::lib::time::TickWatch "TickWatch",
 * \ref aworx::lib::time::TickSpan "TickSpan" and
 * \ref aworx::lib::time::TicksCalendarTime "TicksCalendarTime".
 *
 * The resolution and accuracy of the values is platform dependent. Especially nanoseconds are
 * deemed to be not accurate and above 100 ns (this was written and fact in 2013).
 **************************************************************************************************/
class Ticks
{
    // friends (namespace init/cleanup methods)
    ALIB_API friend void Init();
    ALIB_API friend void TerminationCleanUp();
    ALIB_API friend void SyncClock();

    // #############################################################################################
    // Type and preprocessor definitions
    // #############################################################################################
    public:
        //! @cond NO_DOX

        #define    ALIB_NANOS_PER_DAY              INT64_C( 86400000000000 )
        #define    ALIB_NANOS_PER_HOUR             INT64_C(  3600000000000 )
        #define    ALIB_NANOS_PER_MINUTE           INT64_C(    60000000000 )
        #define    ALIB_NANOS_PER_SECOND           INT64_C(     1000000000 )
        #define    ALIB_NANOS_PER_MILLISECOND      INT64_C(        1000000 )
        #define    ALIB_NANOS_PER_MICROSECOND      INT64_C(           1000 )

        //! @endcond NO_DOX


    // #############################################################################################
    // static fields and friend methods
    // #############################################################################################
    protected:

        /** The ticks when static constructor is invoked. */
        ALIB_API static          Ticks*                      creationTime;

        /** The frequency of the system timer*/
        ALIB_API static          int64_t                     inaccuracy;

        #if defined( __GLIBCXX__ ) || defined(__APPLE__)
            /** Used for conversion of clock times and monotonic clock timer.
             *  (Not defined under Windows OS) */
            static               rawtime                     diffOfMonotonicClockAndNow;

        #elif defined( _WIN32 )
            /** Used for conversion of clock times and monotonic clock timer.
             *  (Only defined under Windows OS) */
            ALIB_API static      int64_t            ticksPerSecond;
        #endif

    // #############################################################################################
    // protected fields
    // #############################################################################################
    protected:
        /// The value
                        rawtime                ticks                                       =0L;

    // #############################################################################################
    // static interface
    // #############################################################################################
    public:

        /** ****************************************************************************************
         * The time (in ticks) when the ticker library containing the ticker (ALIB) was initialized.
         * This might be useful to measure the time since the process was started.
         *
         * @return  Initialization time of the library or process
         ******************************************************************************************/
        inline  static const Ticks&           CreationTime()
        {
            return *creationTime;
        }


        /** ****************************************************************************************
         * The reciprocal value of the frequency of the internal tick timer measured in nanoseconds.
         * This value gives an indication of the (in-)accuracy of the time intervals measured
         * with this class (at least if the  underlying system library is reasonably well
         * working in this respect).
         *
         * @return  The minimum number of nanoseconds between two ticks.
         ******************************************************************************************/
        inline  static int64_t                Inaccuracy()
        {
            return inaccuracy;
        }

    // #############################################################################################
    // Constructors
    // #############################################################################################
    public:

        /** ****************************************************************************************
         * Creates a Ticks instance representing the point in time when this constructor was invoked.
         ******************************************************************************************/
        inline
        Ticks()                         { Set();    }

        /** ****************************************************************************************
         * Creates a Ticks instance representing a given point in time or time span in ticks.
         * @param initialTicks The value to copy into this.
         ******************************************************************************************/
        inline
        Ticks( rawtime initialTicks )   { this->ticks=    initialTicks;    }

    // #############################################################################################
    // Interface
    // #############################################################################################
    public:

        /** ****************************************************************************************
         * Sets the actual point in time as the value of this instance.
         ******************************************************************************************/
        void  Set()
        {
            #if defined( __GLIBCXX__ ) || defined(__APPLE__)
                struct timespec t;
                ALIB_ASSERT_RESULT_EQUALS( clock_gettime(CLOCK_MONOTONIC, &t),  0 );

                ticks= ( t.tv_sec * ALIB_NANOS_PER_SECOND  ) + t.tv_nsec ;
            #elif defined( _WIN32 )
                    LARGE_INTEGER v;
                    ALIB_ASSERT_RESULT_NOT_EQUALS( QueryPerformanceCounter( &v ) , 0 );
                    ticks= v.QuadPart << 1;
            #else
                #pragma message ("Unknown Platform in file: " __FILE__ )
            #endif
        }


        /** ****************************************************************************************
         * Sets the point in time or time span represented by this instance to the value
         * represented by the given Ticks instance.
         * @param other The Ticks object to copy.
         ******************************************************************************************/
        inline
        void         Set( const Ticks& other )      { this->ticks=    other.ticks;    }

        /** ****************************************************************************************
         * Sets this objects' value to the value specified in raw ticks.
         * @param value The number of ticks this object should represent.
         ******************************************************************************************/
        inline
        void         SetRaw( rawtime value )        { this->ticks=    value;           }

        /** ****************************************************************************************
         * Gets the internally stored system dependent time in raw ticks.
         * @return  The internal value
         ******************************************************************************************/
        inline
        rawtime Raw()    const                      { return ticks;                    }

        /** ****************************************************************************************
         * Adds time (span) represented by the given Ticks instance to this instance.
         * @param other The Ticks object to add.
         ******************************************************************************************/
        inline
        void         Add( const Ticks& other )      { this->ticks+=    other.ticks;   }

        /** ****************************************************************************************
         * Adds time (span) represented by the given Ticks instance to this instance.
         * @param other The ticks to add.
         ******************************************************************************************/
        inline
        void         Add( rawtime other )           { this->ticks+=    other;         }

        /** ****************************************************************************************
         * Subtracts the point in time or time span represented by the given Ticks instance from
         * this instance.
         * @param other The Ticks object to subtract.
         ******************************************************************************************/
        inline
        void         Sub( const Ticks& other )      { this->ticks-=    other.ticks;   }

        /** ****************************************************************************************
         * Subtracts the point in time or time span represented by the given Ticks instance from
         * this instance.
         * @param other The ticks to subtract.
         ******************************************************************************************/
        inline
        void         Sub( rawtime other )           { this->ticks-=    other;         }

    // #############################################################################################
    // Interface Age, Since
    // #############################################################################################

        /** ****************************************************************************************
         * Returns the time span between value represented by this instance and the current system
         * time.
         * If the internal value represents a historic point in time, the result is positive.
         *
         * @return  The age of this instance stored in a new Ticks.
         ******************************************************************************************/
        inline
        Ticks    Age()
        {
            Ticks result;
            result.Sub( ticks );
            return result;
        }

        /** ****************************************************************************************
         * Returns the time span between the value represented by this instance and the given
         * StopWatch.If the given StopWatch represents an earlier point in time, the result is
         * positive.
         *
         * @param olderTime The value to compare this instance with
         *
         * @return  The age of this instance stored in the given or created object.
         ******************************************************************************************/
        inline
        Ticks    Since( const Ticks& olderTime )    const
        {
            Ticks result(ticks);
            result.Sub( olderTime );
            return result;
        }

        /** ****************************************************************************************
         * Determines if this objects' age is higher than a given time span.
         *
         * @param waitTime  A \b %Ticks object representing a time span.
         * @return  \c true if the given \b %Ticks object representing a time span is smaller
         *          or equal than our age. \c false otherwise.
         ******************************************************************************************/
        inline
        bool   HasElapsed( Ticks waitTime )
        {
            return Age().Raw()  >=  waitTime.Raw();
        }

    // #############################################################################################
    // Conversion to/from time values (nanoseconds, milliseconds, microseconds, seconds)
    // #############################################################################################
    public:

        /** ****************************************************************************************
         * Converts the internal value to days.
         * @return  The internal value converted to days.
         ******************************************************************************************/
        inline
        int           InDays()       const
        {
            #if defined (__GLIBCXX__) || defined(__APPLE__)
                return static_cast<int>( ticks /    ALIB_NANOS_PER_DAY );
            #elif defined( _WIN32 )
                return    (int) ( ticks /    (ticksPerSecond * 60 * 60 * 24) );
            #else
                #pragma message ("Unknown Platform in file: " __FILE__ )
            #endif

        }

        /** ****************************************************************************************
         * Converts the internal value to hours.
         * @return  The internal value converted to hours.
         ******************************************************************************************/
        inline
        int           InHours()      const
        {
            #if defined (__GLIBCXX__) || defined(__APPLE__)
                return static_cast<int>( ticks /    ALIB_NANOS_PER_HOUR );
            #elif defined( _WIN32 )
                return    (int) ( ticks /    (ticksPerSecond * 60 * 60 ) );
            #else
                #pragma message ("Unknown Platform in file: " __FILE__ )
            #endif
        }

        /** ****************************************************************************************
         * Converts the internal value to minutes.
         * @return  The internal value converted to minutes.
         ******************************************************************************************/
        inline
        int           InMinutes()    const
        {
            #if defined (__GLIBCXX__) || defined(__APPLE__)
                return static_cast<int>( ticks /    ALIB_NANOS_PER_MINUTE );
            #elif defined( _WIN32 )
                return    (int) ( ticks /    (ticksPerSecond * 60) );
            #else
                #pragma message ("Unknown Platform in file: " __FILE__ )
            #endif
        }

        /** ****************************************************************************************
         * Converts the internal value to seconds.
         * @return  The internal value converted to seconds.
         ******************************************************************************************/
        inline
        int           InSeconds()    const
        {
            #if defined (__GLIBCXX__) || defined(__APPLE__)
                return    static_cast<int>(ticks /    ALIB_NANOS_PER_SECOND);
            #elif defined( _WIN32 )
                return    (int) ( ticks /    Ticks::ticksPerSecond );
            #else
                #pragma message ("Unknown Platform in file: " __FILE__ )
            #endif
        }

        /** ****************************************************************************************
         * Converts the internal value to milliseconds.
         * @return  The internal value converted to milliseconds.
         ******************************************************************************************/
        inline
        int64_t  InMillis()    const
        {
            #if defined (__GLIBCXX__) || defined(__APPLE__)
                return    ticks /    ALIB_NANOS_PER_MILLISECOND;
            #elif defined( _WIN32 )
                return    ( ticks * 1000 /   Ticks::ticksPerSecond );

            #else
                #pragma message ("Unknown Platform in file: " __FILE__ )
            #endif
        }

        /** ****************************************************************************************
         * Converts the internal value to microseconds.
         * @return  The internal value converted to microseconds.
         ******************************************************************************************/
        inline
        int64_t  InMicros()    const
        {
            #if defined (__GLIBCXX__) || defined(__APPLE__)
                return    ticks /    ALIB_NANOS_PER_MICROSECOND;
            #elif defined( _WIN32 )
                return    ( ticks * 1000000 /   Ticks::ticksPerSecond );
            #else
                #pragma message ("Unknown Platform in file: " __FILE__ )
            #endif
        }

        /** ****************************************************************************************
         * Converts the internal value to nanoseconds.
         * @return  The internal value converted to nanoseconds.
         ******************************************************************************************/
        inline
        int64_t  InNanos ()    const
        {
            #if defined (__GLIBCXX__) || defined(__APPLE__)
                return    ticks;
            #elif defined( _WIN32 )
                return    (int64_t) ( ticks * 1000000000.0 /   (double) Ticks::ticksPerSecond );
            #else
                #pragma message ("Unknown Platform in file: " __FILE__ )
            #endif
        }

        /** ****************************************************************************************
         * Returns 1 divided by internal value in seconds, hence the number of Hertz that this
         * object represents when interpreted as a time span.
         *
         * @param nDigitsPrecision    Number of digits that the return value will be rounded to.
         *                            Defaults to -1 which means no rounding.
         * @return     double value representing the frequency in hertz.
         ******************************************************************************************/
        ALIB_API
        double       InHertz( int nDigitsPrecision= -1 ) const;

        /** ****************************************************************************************
         * Sets the internal value to a time span provided in seconds.
         * @param days  The time span to set in days.
         * @return   \c *this to allow concatenated calls.
         ******************************************************************************************/
        inline
        Ticks&       FromDays   ( int           days )
        {
            #if defined (__GLIBCXX__) || defined(__APPLE__)
                ticks=                   days *    ALIB_NANOS_PER_DAY   ;
            #elif defined( _WIN32 )
                ticks= (rawtime) (  ((int64_t) days) * 24 * 60 * 60 *  Ticks::ticksPerSecond + 1  );
            #else
                #pragma message ("Unknown Platform in file: " __FILE__ )
            #endif
            return *this;
        }

        /** ****************************************************************************************
         * Sets the internal value to a time span provided in seconds.
         * @param hours  The time span to set in hours.
         * @return   \c *this to allow concatenated calls.
         ******************************************************************************************/
        inline
        Ticks&       FromHours  ( int           hours )
        {
            #if defined (__GLIBCXX__) || defined(__APPLE__)
                ticks=                   hours *    ALIB_NANOS_PER_HOUR   ;
            #elif defined( _WIN32 )
                ticks= (rawtime) (  ((int64_t) hours) * 60 * 60 *  Ticks::ticksPerSecond    + 1 );
            #else
                #pragma message ("Unknown Platform in file: " __FILE__ )
            #endif
            return *this;
        }

        /** ****************************************************************************************
         * Sets the internal value to a time span provided in seconds.
         * @param mins  The time span to set in minutes.
         * @return   \c *this to allow concatenated calls.
         ******************************************************************************************/
        inline
        Ticks&       FromMinutes( int           mins )
        {
            #if defined (__GLIBCXX__) || defined(__APPLE__)
                ticks=                   mins *    ALIB_NANOS_PER_MINUTE   ;
            #elif defined( _WIN32 )
                ticks= (rawtime) (  ((int64_t) mins) * 60 *  Ticks::ticksPerSecond  + 1  );
            #else
                #pragma message ("Unknown Platform in file: " __FILE__ )
            #endif
            return *this;
        }

        /** ****************************************************************************************
         * Sets the internal value to a time span provided in seconds.
         * @param secs  The time span to set in seconds.
         * @return   \c *this to allow concatenated calls.
         ******************************************************************************************/
        inline
        Ticks&       FromSeconds( int           secs )
        {
            #if defined (__GLIBCXX__) || defined(__APPLE__)
                ticks=                   secs *    ALIB_NANOS_PER_SECOND   ;
            #elif defined( _WIN32 )
                ticks= (rawtime) (  ((int64_t) secs) *  Ticks::ticksPerSecond  + 1  );
            #else
                #pragma message ("Unknown Platform in file: " __FILE__ )
            #endif
            return *this;
        }

        /** ****************************************************************************************
         * Sets the internal value to a time span provided in milliseconds.
         * @param millis    The time span to set in milliseconds.
         * @return   \c *this to allow concatenated calls.
         ******************************************************************************************/
        inline
        Ticks&       FromMillis ( int64_t  millis )
        {
            #if defined (__GLIBCXX__) || defined(__APPLE__)
                ticks=                   millis *    ALIB_NANOS_PER_MILLISECOND   ;
            #elif defined( _WIN32 )
                ticks= (rawtime) (  ((int64_t) millis) *  Ticks::ticksPerSecond / 1000   + 1 );
            #else
                #pragma message ("Unknown Platform in file: " __FILE__ )
            #endif
            return *this;
        }

        /** ****************************************************************************************
         * Sets the internal value to a time span provided in microseconds.
         * @param micros  The time span to set in microseconds.
         * @return   \c *this to allow concatenated calls.
         ******************************************************************************************/
        inline
        Ticks&       FromMicros ( int64_t  micros )
        {
            #if defined (__GLIBCXX__) || defined(__APPLE__)
                ticks=                   micros *    ALIB_NANOS_PER_MICROSECOND ;
            #elif defined( _WIN32 )
                ticks= (rawtime) (  ((int64_t) micros) *  Ticks::ticksPerSecond / 1000000  + 1 );
            #else
                #pragma message ("Unknown Platform in file: " __FILE__ )
            #endif
            return *this;
        }

        /** ****************************************************************************************
         * Sets the internal value to a time span provided in nanoseconds.
         * @param nanos  The time span to set in nanoseconds.
         * @return   \c *this to allow concatenated calls.
         ******************************************************************************************/
        inline
        Ticks&     FromNanos  ( int64_t  nanos )
        {
            #if defined (__GLIBCXX__) || defined(__APPLE__)
                ticks=    nanos;
            #elif defined( _WIN32 )
                ticks=  (rawtime) (  nanos / 1000000000.0 *  (double) Ticks::ticksPerSecond );
            #else
                #pragma message ("Unknown Platform in file: " __FILE__ )
            #endif
            return *this;
        }

        /** ****************************************************************************************
         * Formats the point in time represented by this instance into an AString using the given
         * pattern. The token definitions of the format string is equal to those found in
         * \ref TicksCalendarTime.
         *
         * @param format  The format pattern string.
         * @param target  A reference to an AString that gets the result of the format processing
         *                appended.
         * @returns \p target (for convenience).
         ******************************************************************************************/
        ALIB_API
        AString&  Format( const String& format, AString& target );


    // #############################################################################################
    // Conversion to time platform/language specific values
    // #############################################################################################
        #if defined (__GLIBCXX__) || defined(__APPLE__)
            /** ************************************************************************************
             * Converts the internal value into seconds since January 1, 1970, 00:00:00 GMT.
             * The conversion is dependent on time zone and system clock setting of the host.
             * Changes in the system clock setting of the host are reflected only with explicit
             * invocation of namespace function #SyncClock.
             * \note  GLib specific. For Windows OS see #InSystemTime and #InFileTime.
             *
             * @return Seconds in the epoch.
             **************************************************************************************/
            ALIB_API  time_t  InEpochSeconds()    const;

            /** ************************************************************************************
             * Sets the internal value by converting the given seconds since January 1,
             * 1970, 00:00:00 GMT. The conversion is dependent on time zone and system clock
             * setting of the host.
             * Changes in the system clock setting of the host are reflected only with explicit
             * invocation of namespace function #SyncClock.
             * \note  GLib specific. For Windows OS see #SetFromSystemTime and #SetFromFileTime.
             *
             * @param epochSeconds The milliseconds in the epoch to convert.
             **************************************************************************************/
            ALIB_API  void    SetFromEpochSeconds ( time_t epochSeconds );
        #endif // no else here (doxygen)!

        #if defined (_WIN32)
            /** ************************************************************************************
             * Converts the internal value into windows specific file time, a 64-bit value that
             * represents the number of 100 nanosecond intervals that have elapsed since
             * 12:00 A.M. January 1, 1601 UTC. The conversion is dependent on time zone and system
             * clock setting of the host.
             * Changes in the system clock setting of the host are reflected only with explicit
             * invocation of namespace function #SyncClock.
             * \note  Microsoft Windows specific. On other systems see #InEpochSeconds.
             *
             * @param result Pointer to a FILETIME struct to store the result in.
             **************************************************************************************/
            ALIB_API  void    InFileTime( LPFILETIME result ) const;

            /** ************************************************************************************
             * Converts the internal value into windows specific file time, a 64-bit value that
             * represents the number of 100 nanosecond intervals that have elapsed since
             * 12:00 A.M. January 1, 1601 UTC. The conversion is dependent on time zone and system
             * clock setting of the host.
             * Changes in the system clock setting of the host are reflected only with explicit
             * invocation of namespace function #SyncClock.
             * \note  Microsoft Windows specific. On other systems see #InEpochSeconds.
             *
             * @param result A reference to a LARGE_INTEGER struct to store the result in.
             **************************************************************************************/
            ALIB_API  void    InFileTime( ULARGE_INTEGER& result ) const;

            /** ************************************************************************************
             * Sets the internal value to represent the point in time provided as "file time".
             * File time is a 64-bit value that represents the number of 100 nanosecond intervals
             * that have elapsed since 12:00 A.M. January 1, 1601 UTC.
             * The conversion is dependent on time zone and system clock setting of the host.
             * Changes in the system clock setting of the host are reflected only with explicit
             * invocation of namespace function #SyncClock.
             * \note  Microsoft Windows specific. On other systems see #SetFromEpochSeconds.
             *
             * @param fileTime The file time to set.
             **************************************************************************************/
            ALIB_API  void    SetFromFileTime( const FILETIME& fileTime );

            /** ************************************************************************************
             * Sets the internal value to represent the point in time provided as "file time".
             * File time is a 64-bit value that represents the number of 100 nanosecond intervals
             * that have elapsed since 12:00 A.M. January 1, 1601 UTC.
             * The conversion is dependent on time zone and system clock setting of the host.
             * Changes in the system clock setting of the host are reflected only with explicit
             * invocation of namespace function #SyncClock.
             * \note  Microsoft Windows specific. On other systems see #SetFromEpochSeconds.
             *
             * @param fileTime The file time to set.
             **************************************************************************************/
            ALIB_API  void    SetFromFileTime( const ULARGE_INTEGER& fileTime );

            /** ************************************************************************************
             * Converts the internal value into windows specific system time struct.
             * The conversion is dependent on time zone and system clock setting of the host.
             * Changes in the system clock setting of the host are reflected only with explicit
             * invocation of namespace function #SyncClock.
             * \note  Microsoft Windows specific. On other systems see #SetFromEpochSeconds.
             *
             * @param result   Pointer to a SYSTEMTIME struct to store the result in.
             * @param timezone Denote if the time that is returned should be local or UTC.
             *                 Defaults to \c TimeZone::Local.
             * @return Seconds in the epoch.
             **************************************************************************************/
            ALIB_API  void    InSystemTime( LPSYSTEMTIME result, lang::Timezone timezone =lang::Timezone::Local ) const;

            /** ************************************************************************************
             * Sets the internal value to represent the point in time provided as windows system
             * time. The conversion is dependent on time zone and system clock setting of the host.
             * Changes in the system clock setting of the host are reflected only with explicit
             * invocation of namespace function #SyncClock.
             * \note  Microsoft Windows specific. On other systems see #SetFromEpochSeconds.
             *
             * @param systemTime Pointer to a SYSTEMTIME struct that holds the system time to set.
             * @param timezone Denote if the time is interpreted as local or UTC.
             *                 Defaults to \c TimeZone::Local.
             **************************************************************************************/
            ALIB_API  void    SetFromSystemTime( const SYSTEMTIME& systemTime, lang::Timezone timezone= lang::Timezone::Local );
        #endif


};



#if ALIB_MODULE_BOXING

/**
 * Implementation of \ref aworx::lib::strings::boxing::IFormat "IFormat" for boxable type
 * <c>Ticks*</c>.<br>
 * Writes the content of \p box (which is of type \b %Ticks*) to the given \b %AString
 * object \p target using a local instance of class \b %TicksCalendarTime and its method
 * \ref aworx::lib::time::TicksCalendarTime::Format "TicksCalendarTime::Format".
 *
 */
class IFormat_TTicks: public strings::boxing::IFormat, public Singleton<IFormat_TTicks>
{
    /**
     * Interface implementation.
     * @param box   The box that the interface was invoked on.
     * @param formatSpec The specification of the format (type specific)
     * @param target     The AString object receiving the formatted string.
     */
    ALIB_API
    virtual void Invoke( const Box& box, const String& formatSpec, AString& target )                  ALIB_OVERRIDE;
};

void InitBoxing();
/**
 * Initializes the <b>ALib %Boxing</b> system in respect to boxing types found in namespace
 * \ref aworx::lib::time.<br>
 * Currently, the only action taken is the registration of interface class
 * \ref aworx::lib::time::IFormat_TTicks "IFormat_TTicks" with boxer
 * \ref aworx::lib::boxing::BoxerT "BoxerT<time::Ticks*>".
 *
 * This method is invoked by
  \ref aworx::lib::ALIB::Init "ALIB::Init".
 */
inline void InitBoxing()
{
    boxing::DefineInterface<time::Ticks*, false, time::IFormat_TTicks>();
}
#endif

}} // namespace lib::time

/** Type alias name in namespace #aworx. */
using     rawtime=              aworx::lib::time::rawtime;

/** Type alias name in namespace #aworx. */
using     Ticks=                aworx::lib::time::Ticks;

}  // namespace aworx

#endif // HPP_ALIB_TIME_TICKS