#ifndef XLSX_WORKBOOK_H
#define XLSX_WORKBOOK_H

#include "SimpleXlsxDef.h"

#include "Chartsheet.h"
#include "Worksheet.h"

namespace SimpleXlsx
{
    class CChart;
    class CDrawing;

    class PathManager;
    class XMLWriter;

    // ****************************************************************************
    /// @brief	The class CWorkbook is used to manage creation, population and saving .xlsx files
    // ****************************************************************************
    class CWorkbook
    {
            _tstring                    m_temp_path;		///< path to the temporary directory (unique for a book)
            std::vector<CWorksheet *>   m_worksheets;		///< a series of data sheets
            std::vector<CChartsheet *>  m_chartsheets;		///< a series of chart sheets
            std::vector<CChart *>       m_charts;           ///< a series of charts
            std::vector<CDrawing *>     m_drawings;         ///< a series of drawings
            std::map<std::string, uint64_t> m_sharedStrings;///<
            std::vector<Comment>		m_comments;			///<

            size_t                      m_commLastId;		///< m_commLastId comments counter
            _tstring                    m_UserName;
            size_t                      m_sheetId;          ///< Current sheet sequence number (for sheets ordering)
            size_t                      m_activeSheetIndex; ///< Index of active (opened) sheet

            StyleList                   m_styleList;        ///< All registered styles

            PathManager        *        m_pathManager;      ///<

        public:
            // @section    Constructors / destructor
            CWorkbook( const _tstring & UserName = _T( "" ) );
            virtual ~CWorkbook();

            // @section    User interface

            //Adds another data sheet into the workbook
            CWorksheet & AddSheet( const _tstring & title );
            CWorksheet & AddSheet( const _tstring & title, std::vector<ColumnWidth> & colWidths );
            CWorksheet & AddSheet( const _tstring & title, uint32_t frozenWidth, uint32_t frozenHeight );
            CWorksheet & AddSheet( const _tstring & title, uint32_t frozenWidth, uint32_t frozenHeight, std::vector<ColumnWidth> & colWidths );

            //Adds chart into the data sheet
            CChart & AddChart( CWorksheet & sheet, ChartPoint TopLeft, ChartPoint BottomRight, EChartTypes type = CHART_LINEAR );

            //Adds sheet with single chart
            CChartsheet & AddChartSheet( const _tstring & title, EChartTypes type = CHART_LINEAR );

            // *INDENT-OFF*   For AStyle tool

            //Adds a new style into collection if it is not exists yet.
            //Return style index that should be used at data appending to a data sheet.
            inline size_t AddStyle( const Style & style )           { return m_styleList.Add( style ); }
            //Vector with exist fonts
            inline const std::vector<Font> & GetFonts()	const       { return m_styleList.GetFonts(); }

            //Get active (opened) sheet
            inline size_t GetActiveSheetIndex() const               { return m_activeSheetIndex; }
            //Set active (opened) sheet (start from 0).
            inline void SetActiveSheet( size_t index )              { m_activeSheetIndex = index; }
            inline void SetActiveSheet( const CWorksheet & sheet )  { m_activeSheetIndex = sheet.GetIndex() - 1; }
            inline void SetActiveSheet( const CChartsheet & sheet ) { m_activeSheetIndex = sheet.GetIndex() - 1; }

            // *INDENT-ON*   For AStyle tool

            //Save current workbook
            bool Save( const _tstring & name );

        private:
            //Disable copy and assignment
            CWorkbook( const CWorkbook & that );
            CWorkbook & operator=( const CWorkbook & );

            CWorksheet & InitWorkSheet( CWorksheet * sheet, const _tstring & title );
            CDrawing * CreateDrawing();

            bool SaveCore();
            bool SaveContentType();
            bool SaveApp();
            bool SaveTheme();
            bool SaveStyles();
            bool SaveChain();
            bool SaveComments();
            bool SaveSharedStrings();
            bool SaveWorkbook();

            bool SaveCommentList( const std::vector<Comment *> & comments );
            void AddComment( XMLWriter & xmlw, const Comment & comment ) const;
            void AddCommentDrawing( XMLWriter & xmlw, const Comment & comment );
            void AddNumberFormats( XMLWriter & xmlw ) const;
            void AddFonts( XMLWriter & xmlw ) const;
            void AddFills( XMLWriter & xmlw ) const;
            void AddBorders( XMLWriter & xmlw ) const;
            void AddBorder( XMLWriter & xmlw, const char * borderName, Border::BorderItem border ) const;
            void AddFontInfo( XMLWriter & xmlw, const Font & font, const char * FontTagName, int32_t Charset ) const;

            static std::string GetFormatCodeString( const NumFormat & fmt );
            static std::string GetFormatCodeColor( ENumericStyleColor color );
            static std::string CurrencySymbol();
    };

}	// namespace SimpleXlsx

#endif	// XLSX_WORKBOOK_H
