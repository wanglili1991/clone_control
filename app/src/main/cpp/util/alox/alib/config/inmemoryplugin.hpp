// #################################################################################################
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
#if defined(HPP_COM_ALIB_TEST_INCLUDES) && defined(HPP_ALIB_CONFIG_INMEMORY_PLUGIN)
    #error "Header already included"
#endif

// then, set include guard
#ifndef HPP_ALIB_CONFIG_INMEMORY_PLUGIN
//! @cond NO_DOX
#define HPP_ALIB_CONFIG_INMEMORY_PLUGIN 1
//! @endcond NO_DOX


// #################################################################################################
// includes
// #################################################################################################
#if !defined (HPP_ALIB_CONFIG_PLUGINS)
    #include "alib/config/plugins.hpp"
#endif


#if !defined (_GLIBCXX_VECTOR ) && !defined(_VECTOR_)
    #include <vector>
#endif


// #################################################################################################
// forward declarations
// #################################################################################################
namespace aworx { namespace lib { namespace strings { class Substring; } } }


namespace aworx { namespace lib { namespace config
{
/** ************************************************************************************************
 * Specialization of abstract interface class #ConfigurationPlugin, which holds
 * configuration variables in memory. Instances of this class are used to create two plug-ins
 * within standard configurations:
 * - One with priority
 *   \ref aworx::lib::config::Configuration::PrioDefault "PrioDefault" (low priority)
 *   to store default values not found in other configuration plug-ins and
 * - a second with priority
 *   \ref aworx::lib::config::Configuration::PrioProtected "PrioProtected" (highest priority),
 *   which can be used protect configuration values from external change.
 *
 * This class in addition acts as the parent of class
 * \ref aworx::lib::config::IniFile "IniFile" and potentially other (custom) classes.
 * For this reason, it divides the set of variables into sections (according to the category),
 * allows comment strings for variables and sections, and virtualizes some key methods to
 * allow descendants to take specific actions.
 *
 * This class offers important internal fields and types for public access (bauhaus code style).
 * However, in standard cases, only the interface methods of this class should be used.
 **************************************************************************************************/
 class InMemoryPlugin : public ConfigurationPlugin
{
    // #############################################################################################
    // inner classes
    // #############################################################################################
    public:
        /** ****************************************************************************************
         * A configuration sections' entry
         ******************************************************************************************/
        class Entry
        {

            public:
                String32             Name;                ///< The name of the section.
                AString              Comments;            ///< The comments of the section.
                char                 Delim = '\0';        ///< The delimiter (for splitting output into to multiple lines).
                int                  FormatHints= 0;      ///< The format hints of the variable
                String16             FormatAttrAlignment; ///< The format attribute alignment hint of the variable

                std::vector<AString> Values;              ///< The list of values of the entry of the section.

                /**
                 * Constructs an Entry
                 * @param name    The name of the section.
                 */
                Entry( const String& name )
                {
                    ALIB_WARN_ONCE_PER_INSTANCE_DISABLE( Name,  ReplaceExternalBuffer );
                    Name._( name );
                }
                /** Destructor */
                virtual ~Entry()   {}

                /**
                 * Virtual method that copies the values of an entry to the given \p variable.
                 *
                 * @param parent    The plug-in we belong to.
                 * @param variable  The variable to fill with our values.
                 */
                ALIB_API
                virtual void ToVariable( const InMemoryPlugin* parent, Variable& variable );

                /**
                 * Virtual method that copies the values of the given \p variable to us.
                 *
                 * @param parent    The plug-in we belong to.
                 * @param variable  The variable to fill with our values.
                 */
                ALIB_API
                virtual void FromVariable( const InMemoryPlugin* parent, Variable& variable );
        };

        /** ****************************************************************************************
         * A configuration section
         ******************************************************************************************/
        class Section
        {
            public:
                String32             Name;         ///< The name of the section.
                AString              Comments;     ///< The comments of the section.

                std::vector<Entry*>  Entries;      ///< The list of variables of the section.

            /**
             * Constructs a Section
             * @param name    The name of the section.
             */
            Section( const String& name )
            : Name( name )
            {
                ALIB_WARN_ONCE_PER_INSTANCE_DISABLE( Name,  ReplaceExternalBuffer);
            }

            /** Destructor */
            virtual ~Section()
            {
                for ( Entry* entry : Entries )
                    delete entry;
            }

            /**
             * Searches an entry with the given name. The search is performed case insensitive
             * @param name      The name of the variable to be searched.
             * @param create    Denotes if an entry should be created if not found.
             * @return The variable if found, else \c nullptr.
             */
            ALIB_API
            Entry*     GetEntry ( const String& name, bool create );

            /**
             * Deletes an entry (if exists)
             * @param name      The name of the variable to be searched.
             * @return \c true if the entry was found and deleted, \c false otherwise.
             */
            ALIB_API
            bool       DeleteEntry ( const String& name );

            /** ************************************************************************************
             * Virtual method to create an Entry.
             * (Provided to allow descendant classes to created extended versions of an entry. See
             * also \ref aworx::lib::config::InMemoryPlugin::createSection "InMemoryPlugin::createSection".)
             *
             * @param  name The name of the entry.
             * @return An object of type \ref Entry "InMemoryPlugin::Entry".
             **************************************************************************************/
            protected:
            inline
            virtual Entry*  createEntry( const String& name )
            {
                return new Entry( name );
            }
        };

    // #############################################################################################
    // Public fields
    // #############################################################################################
    public:
        /** The list of sections. */
        std::vector<Section*>            Sections;

    // #############################################################################################
    // Constructor/destructor
    // #############################################################################################
    public:
        /** ****************************************************************************************
         * Constructor.
         ******************************************************************************************/
        inline        InMemoryPlugin() { Reset(); }

        /** ****************************************************************************************
         * Virtual Destructor.
         ******************************************************************************************/
        ALIB_API
        virtual      ~InMemoryPlugin();

    // #############################################################################################
    // Interface
    // #############################################################################################
        using ConfigurationPlugin::Load;
        using ConfigurationPlugin::Store;

        /** ****************************************************************************************
         * Clears all configuration data.
         ******************************************************************************************/
        ALIB_API
        virtual void    Reset();

        /** ****************************************************************************************
         * Searches the \ref aworx::lib::config::InMemoryPlugin::Section "Section" with the given name.
         *
         * @param name      The name of the section to be retrieved.
         * @return Returns the section if it was found, nullptr otherwise.
         ******************************************************************************************/
        ALIB_API
        Section*        SearchSection( const String& name)  const;

        /** ****************************************************************************************
         * Searches the \ref aworx::lib::config::InMemoryPlugin::Section "Section" with the given
         * name. If the section was not found, it is created.
         * If the section is found and has no comments, then the provided comments are appended.
         * @param name      The name of the section to be retrieved.
         * @param comments  The comment lines for the section, in the case the section is not
         *                  found. If this is nulled, no section is created.
         * @return Returns the section if it was found or created. nullptr otherwise.
         ******************************************************************************************/
        ALIB_API
        Section*        SearchOrCreateSection( const String& name, const String& comments );

    // #############################################################################################
    // ConfigurationPlugin interface implementation
    // #############################################################################################
    public:

        /** ****************************************************************************************
         * Searches the variable in our storage.
         *
         * @param variable  The variable to retrieve.
         * @param searchOnly   If \c true, the variable is not read. Defaults to \c false.
         * @return \c true if variable was found, \c false if not.
         ******************************************************************************************/
        ALIB_API
        virtual bool    Load( Variable& variable, bool searchOnly= false )  const;

        /** ****************************************************************************************
         * Creates or replaces existing variable in our storage.
         *
         * @param variable  The variable to retrieve.
         * @return \c true if the variable was written, \c false if not. The latter might only
         *         happen if the variable given was illegal, e.g. empty name.
         ******************************************************************************************/
        ALIB_API
        virtual bool    Store( Variable& variable );


    // #############################################################################################
    // Internals
    // #############################################################################################
    protected:
        /** ****************************************************************************************
         * Virtual method to create a section.
         * (Provided to allow descendant classes to created extended versions of an entry.)
         * @param  name The name of the section
         * @return An object of type \ref Section "InMemoryPlugIn::Section".
         ******************************************************************************************/
        inline
        virtual Section*  createSection( const String& name )
        {
            return new InMemoryPlugin::Section( name );
        }

};


}} // namespace lib::config

/** Type alias name in namespace #aworx. */
using     InMemoryPlugin=       aworx::lib::config::InMemoryPlugin;

}  // namespace aworx

#endif // HPP_ALIB_CONFIG_INMEMORY_PLUGIN
