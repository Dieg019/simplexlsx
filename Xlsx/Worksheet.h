#ifndef XLSX_WORKSHEET_H
#define XLSX_WORKSHEET_H

#include <list>
#include <map>
#include <string>
#include <vector>

#include "SimpleXlsxDef.h"
#include "../XMLWriter.hpp"

namespace SimpleXlsx
{
    class CDrawing;

    class PathManager;

    // ****************************************************************************
    /// @brief	The class CWorksheet is used for creation and population
    ///         .xlsx file sheet table with data
    // ****************************************************************************
    class CWorksheet
    {
        public:
            enum EPageOrientation
            {
                PAGE_PORTRAIT = 0,
                PAGE_LANDSCAPE
            };

        private:
            XMLWriter       *       m_XMLWriter;        ///< xml output stream
            std::vector<std::string>m_calcChain;        ///< list of cells with formulae
            std::map<std::string, uint64_t> * m_sharedStrings; ///< pointer to the list of string supposed to be into shared area
            std::vector<Comment> *	m_comments;         ///< pointer to the vector of comments
            std::list<std::string>  m_mergedCells;	///< list of merged cells` ranges (e.g. A1:B2)
            size_t					m_index;            ///< current sheet number
            _tstring             	m_title;            ///< page title
            bool                    m_withFormula;      ///< indicates whether the sheet contains formulae
            bool					m_withComments;		///< indicates whether the sheet contains any comments
            bool                    m_isOk;             ///< indicates initialization successfulness
            bool                    m_isDataPresented;  ///< indicates whether the sheet contains a data
            uint32_t				m_row_index;        ///< since data add row-by-row it contains current row to write
            bool					m_row_opened;		///< indicates whether row tag is opened
            uint32_t				m_current_column;	///< used at separate row generation - last cell column number to be added
            uint32_t				m_offset_column;	///< used at entire row addition (implicit parameter for AddCell method)

            EPageOrientation		m_page_orientation;	///< defines page orientation for printing

            PathManager      &      m_pathManager;      ///< reference to XML PathManager
            CDrawing        &       m_Drawing;          ///< Reference to drawing object

        public:
            // *INDENT-OFF*   For AStyle tool

            // @section    SEC_INTERNAL Interclass internal interface methods
            inline bool     IsThereComment() const      { return m_withComments; }
            inline bool     IsThereFormula() const      { return m_withFormula; }
            inline size_t   GetIndex() const            { return m_index; }
            inline void     GetCalcChain( std::vector<std::string> & chain ) const  { chain = m_calcChain; }

            // @section    SEC_USER User interface
            inline _tstring GetTitle() const                { return m_title; }
            inline void SetTitle( const _tstring & title )  { if( ! title.empty() ) m_title = title; }

            inline void SetPageOrientation( EPageOrientation orient ) { m_page_orientation = orient; }

            // *INDENT-ON*   For AStyle tool

            void AddComment( const Comment & comment )
            {
                if( m_comments != NULL )
                {
                    m_comments->push_back( comment );
                    m_comments->back().sheetIndex = m_index;
                    m_withComments = true;
                }
            }

            // *INDENT-OFF*   For AStyle tool

            void BeginRow( uint32_t height = 0 );
            inline void AddCell()                                       { m_current_column++; }

            void AddCell( const CellDataStr & data )                    { AddCell( data.value, data.style_id );        }
            void AddCell( const std::string & value, size_t style_id = 0 );
            void AddCell( const char * value, size_t style_id = 0 )     { AddCell( std::string( value ), style_id );   }
#ifdef _UNICODE
            void AddCell( const std::wstring & value, size_t style_id  = 0 ){ AddCell( UTF8Encoder::From_wstring( value ), style_id ); }
#endif
            void AddCells( const std::vector<CellDataStr> & data );

            void AddCell( const CellDataTime & data )                   { AddCell( data.value, data.style_id );        }
            void AddCell( time_t value, size_t style_id );
            void AddCells( const std::vector<CellDataTime> & data );

            void AddCell( const CellDataInt & data )                    { AddCellRoutine( data.value, data.style_id ); }
            void AddCell( int32_t value, size_t style_id = 0 )          { AddCellRoutine( value, style_id );           }
            void AddCells( const std::vector<CellDataInt> & data )      { AddCellsTempl( data ); }

            void AddCell( const CellDataUInt & data )                   { AddCellRoutine( data.value, data.style_id ); }
            void AddCell( uint32_t value, size_t style_id = 0 )         { AddCellRoutine( value, style_id );           }
            void AddCells( const std::vector<CellDataUInt> & data )     { AddCellsTempl( data ); }

            void AddCell( const CellDataDbl & data )                    { AddCellRoutine( data.value, data.style_id ); }
            void AddCell( double value, size_t style_id = 0 )           { AddCellRoutine( value, style_id );           }
            void AddCells( const std::vector<CellDataDbl> & data )      { AddCellsTempl( data ); }

            void AddCell( const CellDataFlt & data )                    { AddCellRoutine( data.value, data.style_id ); }
            void AddCell( float value, size_t style_id = 0 )            { AddCellRoutine( value, style_id );           }
            void AddCells( const std::vector<CellDataFlt> & data )      { AddCellsTempl( data ); }
            void EndRow();

            void AddRow( const std::vector<CellDataStr> & data, uint32_t offset = 0, uint32_t height = 0 )  { AddRowTempl( data, offset, height ); }
            void AddRow( const std::vector<CellDataTime> & data, uint32_t offset = 0, uint32_t height = 0 ) { AddRowTempl( data, offset, height ); }
            void AddRow( const std::vector<CellDataInt> & data, uint32_t offset = 0, uint32_t height = 0 )  { AddRowTempl( data, offset, height ); }
            void AddRow( const std::vector<CellDataUInt> & data, uint32_t offset = 0, uint32_t height = 0 ) { AddRowTempl( data, offset, height ); }
            void AddRow( const std::vector<CellDataDbl> & data, uint32_t offset = 0, uint32_t height = 0 )  { AddRowTempl( data, offset, height ); }
            void AddRow( const std::vector<CellDataFlt> & data, uint32_t offset = 0, uint32_t height = 0 )  { AddRowTempl( data, offset, height ); }

            void MergeCells( CellCoord cellFrom, CellCoord cellTo );

            void GetCurrentCellCoord( CellCoord & currCell ) const;
            inline uint32_t CurrentRowIndex() const     { return m_row_index; }
            inline uint32_t CurrentColumnIndex() const  { return m_current_column; }

            // *INDENT-ON*   For AStyle tool

            bool IsOk() const
            {
                return m_isOk;
            }

        protected:
            CWorksheet( size_t index, CDrawing & drawing, PathManager & pathmanager );
            CWorksheet( size_t index, const std::vector<ColumnWidth> & colHeights, CDrawing & drawing, PathManager & pathmanager );
            CWorksheet( size_t index, uint32_t width, uint32_t height, CDrawing & drawing, PathManager & pathmanager );
            CWorksheet( size_t index, uint32_t width, uint32_t height, const std::vector<ColumnWidth> & colHeights,
                        CDrawing & drawing, PathManager & pathmanager );
            virtual ~CWorksheet();

        private:
            //Disable copy and assignment
            CWorksheet( const CWorksheet & that );
            CWorksheet & operator=( const CWorksheet & );

            bool Save();

            // *INDENT-OFF*   For AStyle tool
            inline void     SetSharedStr( std::map<std::string, uint64_t> * share ) { m_sharedStrings = share; }
            inline void     SetComments( std::vector<Comment> * share )             { m_comments = share; }
            // *INDENT-ON*   For AStyle tool

            void Init( size_t index, uint32_t frozenWidth, uint32_t frozenHeight, const std::vector<ColumnWidth> & colHeights );
            void AddFrozenPane( uint32_t width, uint32_t height );

            template<typename T>
            void AddCellRoutine( T data, size_t style );

            template<typename T>
            void AddCellsTempl( const std::vector<T> & data );

            template<typename T>
            void AddRowTempl( const std::vector<T> & data, uint32_t offset, uint32_t height );

            bool SaveSheetRels();

            friend class CWorkbook;
    };

    // ****************************************************************************
    /// @brief  Appends another a group of cells into a row
    /// @param  data template data value
    /// @param	style style index
    /// @return no
    // ****************************************************************************
    template<typename T>
    void CWorksheet::AddCellRoutine( T data, size_t style )
    {
        m_XMLWriter->Tag( "c" ).Attr( "r", CellCoord( m_row_index, m_offset_column + m_current_column ).ToString() );
        if( style != 0 )    // default style is not necessary to sign explicitly
            m_XMLWriter->Attr( "s", style );
        m_XMLWriter->TagOnlyContent( "v", data ).End( "c" );

        m_current_column++;
    }

    template<typename T>
    void CWorksheet::AddCellsTempl( const std::vector<T> & data )
    {
        for( typename std::vector<T>::const_iterator it = data.begin(); it != data.end(); it++ )
            AddCellRoutine( it->value, it->style_id );
    }

    // ****************************************************************************
    /// @brief  Appends another row into the sheet
    /// @param  data reference to the vector of  <T>
    /// @param  offset the offset from the row begining (0 by default)
    /// @param	height row height (deafult if 0)
    /// @return no
    // ****************************************************************************
    template<typename T>
    void CWorksheet::AddRowTempl( const std::vector<T> & data, uint32_t offset, uint32_t height )
    {
        m_offset_column = offset;
        _tstringstream Spans;
        Spans << m_offset_column + 1 << ':' << data.size() + m_offset_column + 1;
        m_XMLWriter->Tag( "row" ).Attr( "r", ++m_row_index ).Attr( "spans", Spans.str() ).Attr( "x14ac:dyDescent", 0.25 );
        if( height != 0 ) m_XMLWriter->Attr( "ht", height ).Attr( "customHeight", 1 );
        m_current_column = 0;
        AddCells( data );
        m_XMLWriter->End( "row" );

        m_offset_column = 0;
    }

    // ****************************************************************************
    /// @brief	Generates a header for another row
    /// @param	height row height (default if 0)
    /// @return	no
    // ****************************************************************************
    inline void CWorksheet::BeginRow( uint32_t height )
    {
        if( m_row_opened ) m_XMLWriter->End( "row" );
        m_XMLWriter->Tag( "row" ).Attr( "r", ++m_row_index ).Attr( "x14ac:dyDescent", 0.25 );

        if( height != 0 ) m_XMLWriter->Attr( "ht", height ).Attr( "customHeight", 1 );

        m_current_column = 0;
        m_row_opened = true;

    }

    // ****************************************************************************
    /// @brief	Adds a group of cells into a row
    /// @param  data reference to the vector of pairs <string, style_index>
    /// @return	no
    // ****************************************************************************
    inline void CWorksheet::AddCells( const std::vector<CellDataStr> & data )
    {
        for( size_t i = 0; i < data.size(); i++ )
            AddCell( data[i] );
    }

    // ****************************************************************************
    /// @brief	Adds a group of cells into a row
    /// @param  data reference to the vector of pairs <time_t, style_index>
    /// @return	no
    // ****************************************************************************
    inline void CWorksheet::AddCells( const std::vector<CellDataTime> & data )
    {
        for( size_t i = 0; i < data.size(); i++ )
            AddCell( data[i] );
    }

    // ****************************************************************************
    /// @brief	Closes previously began row
    /// @return	no
    // ****************************************************************************
    inline void CWorksheet::EndRow()
    {
        if( ! m_row_opened ) return;
        m_XMLWriter->End( "row" );
        m_row_opened = false;
    }

} // namespace SimpleXlsx

#endif	// XLSX_WORKSHEET_H
