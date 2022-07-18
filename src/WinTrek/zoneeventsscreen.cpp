#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// ZoneEvents Screen
//
//////////////////////////////////////////////////////////////////////////////

class ZoneEventsScreen :
    public Screen,
    public EventTargetContainer<ZoneEventsScreen>,
    public IItemEvent::Sink,
    public IHTTPSessionSink
{
public:

    enum EventsColumn
    {
        colDate,
        colTime,
        colName,
        colDescription,
        maxCol
    };

    class EventListing : public IObject
    {
        ZString m_strDate;
        ZString m_strTime;
        ZString m_strName;
        ZString m_strDescription;
        ZString m_strSignUpURL;
        TList<ZString> m_listFiles;

    public:

        EventListing(IObject* pobjTuple)
        {
            IObjectPair* ptuple;
            CastTo(ptuple, pobjTuple);
            m_strDate = GetString(ptuple->GetFirst());
            m_strTime = GetString(ptuple->GetNth(1));
            m_strName = GetString(ptuple->GetNth(2));
            m_strDescription = GetString(ptuple->GetNth(3));
            m_strSignUpURL = GetString(ptuple->GetNth(4));

            IObjectList* plist;
            CastTo(plist, ptuple->GetLastNth(5));

            plist->GetFirst();
            while (plist->GetCurrent() != NULL) {
                m_listFiles.PushEnd(GetString(plist->GetCurrent()));
                plist->GetNext();
            }
        }

        const ZString& GetDate() const
        {
            return m_strDate;
        }

        const ZString& GetTime() const
        {
            return m_strTime;
        }

        const ZString& GetName() const
        {
            return m_strName;
        }

        const ZString& GetDescription() const
        {
            return m_strDescription;
        }

        const ZString& GetSignUpURL() const
        {
            return m_strSignUpURL;
        }

        TList<ZString>& GetFileList()
        {
            return m_listFiles;
        }
    };

    class EventPainter : public ItemPainter
    {
        const TVector<int>& m_viColumns;
        ZoneEventsScreen* m_pparent;

    public:

        EventPainter(const TVector<int>& viColumns, ZoneEventsScreen* pparent)
            : m_viColumns(viColumns), m_pparent(pparent) {};

        int GetXSize()
        {
            return m_viColumns[3];
        }

        int GetYSize()
        {
            return 12;
        }

        void Paint(ItemID pitemArg, Surface* psurface, bool bSelected, bool bFocus)
        {
            EventListing* pevent = (EventListing*)pitemArg;
            Color colorText(1, 1, 1);

            if (pevent == NULL)
                return;

            if (bSelected) {
                psurface->FillRect(
                    WinRect(0, 0, GetXSize(), GetYSize()),
                    Color(1, 0, 0)
                );
            }

            psurface->DrawString(
                TrekResources::SmallFont(),
                colorText,
                WinPoint(2, 0),
                pevent->GetDate()
            );
            psurface->DrawString(
                TrekResources::SmallFont(),
                colorText,
                WinPoint(2 + m_viColumns[0], 0),
                pevent->GetTime()
            );
            psurface->DrawString(
                TrekResources::SmallFont(),
                colorText,
                WinPoint(2 + m_viColumns[1], 0),
                pevent->GetName()
            );
            psurface->DrawString(
                TrekResources::SmallFont(),
                colorText,
                WinPoint(2 + m_viColumns[2], 0),
                pevent->GetDescription()
            );
        }
    };

    TRef<INameSpace>            m_pns;
    TRef<Pane>                  m_ppane;
    TRef<ButtonPane>            m_pbuttonBack;
    TRef<ButtonPane>            m_pbuttonSignUp;
    TRef<ListPane>              m_plistPaneEvents;
    TRef<IItemEvent::Sink>      m_psinkEvents;
    TRef<IItemEvent::Source>    m_peventEvents;
    TRef<PagePane>              m_ppageDescription;

    TVector<int>                m_viEventsColumns;
    IHTTPSession *              m_pSession;

    bool                        m_bLoadingEventList;
    TRef<EventListing>          m_peventListingPending;
    ZString                     m_strWaitForFile;

    int                         m_nRetriesLeft;
    enum { c_cRetries = 5 };

public:
    ZoneEventsScreen(Modeler* pmodeler) :
      m_bLoadingEventList(false), m_pSession(NULL), m_nRetriesLeft(c_cRetries)
    {
        TRef<IObject> pobjEventsColumns;

        m_pns = pmodeler->GetNameSpace("zoneeventsscreen");
        CastTo(m_ppane, m_pns->FindMember("screen"));

        CastTo(m_pbuttonBack, m_pns->FindMember("backButtonPane"));
        CastTo(m_pbuttonSignUp, m_pns->FindMember("signUpButtonPane"));

        CastTo(m_plistPaneEvents, (Pane*)m_pns->FindMember("eventsListPane"));
        CastTo(pobjEventsColumns, m_pns->FindMember("eventsColumns"));

        CastTo(m_ppageDescription, (Pane*)m_pns->FindMember("descriptionPage"));

        //
        // Buttons
        //
        
		// mdvalley: 2005 needs a pointer and the class spelled out
        AddEventTarget(&ZoneEventsScreen::OnButtonBack, m_pbuttonBack->GetEventSource());
        AddEventTarget(&ZoneEventsScreen::OnButtonSignUp, m_pbuttonSignUp->GetEventSource());


        //
        // Event list
        //

        ParseIntVector(pobjEventsColumns, m_viEventsColumns);
        m_plistPaneEvents->SetItemPainter(new EventPainter(m_viEventsColumns, this));
        m_peventEvents = m_plistPaneEvents->GetSelectionEventSource();
        m_peventEvents->AddSink(m_psinkEvents = new IItemEvent::Delegate(this));
        
        LoadEvents("defaultevents");
        BeginZoneEventsDownload();
        m_pbuttonSignUp->SetHidden(true);


        //
        // PagePanes
        //

        TRef<FontValue>  pfont;
        TRef<ColorValue> pcolor;
        TRef<ColorValue> pcolorHighlight;

        CastTo(pfont,           m_pns->FindMember("descriptionFont"));
        CastTo(pcolor,          m_pns->FindMember("descriptionColor"));
        CastTo(pcolorHighlight, m_pns->FindMember("descriptionHighlightColor"));

        m_ppageDescription->SetAttributes(
            pfont->GetValue(),
            pcolor->GetValue(),
            Color(0,0,1),
            Color(0,0,1),
            pcolorHighlight->GetValue()
        );
        m_ppageDescription->SetTopic(m_pns, "blank");
      } 

    ~ZoneEventsScreen()
    {
        if (m_pSession)
        {
            delete m_pSession;
            m_pSession = NULL;
        }

        m_peventEvents->RemoveSink(m_psinkEvents);
    }

    void UpdateButtons()
    {
    }

    bool OnEvent(IItemEvent::Source *pevent, ItemID pitem)
    {
        if (pitem)
        {
            EventListing* peventListing = (EventListing*)pitem;
            if (peventListing->GetFileList().IsEmpty())
            {
                m_ppageDescription->SetTopic(m_pns, "blank");
            }
            else
            {
                m_ppageDescription->SetTopic(m_pns, "loading");
                BeginEventListingDownload(peventListing);
            }
    
            m_pbuttonSignUp->SetHidden(peventListing->GetSignUpURL().IsEmpty());
        }
        else
        {
            m_ppageDescription->SetTopic(m_pns, "blank");
            m_pbuttonSignUp->SetHidden(true);
        }

        return true;
    }

    bool OnButtonBack()
    {
        GetWindow()->screen(ScreenIDZoneClubScreen);

        return true;
    }

    bool OnButtonSignUp()
    {
        EventListing* peventListing = (EventListing*)m_plistPaneEvents->GetSelection();

        if (peventListing && !peventListing->GetSignUpURL().IsEmpty())
        {
            GetWindow()->ShowWebPage(peventListing->GetSignUpURL());
        }

        return true;
    }

    void LoadEvents(const ZString& strNamespace)
    {
        TRef<INameSpace> pns = GetModeler()->GetNameSpace(strNamespace);
        
        if (pns)
        {
            // create a list just for use 
            TRef<TListListWrapper<TRef<EventListing> > > plistEvents 
                = new TListListWrapper<TRef<EventListing> >();

            // load the list of events
            IObjectList* plist;
            CastTo(plist, pns->FindMember("events"));

            plist->GetFirst();
            while (plist->GetCurrent() != NULL) {
                plistEvents->PushEnd(new EventListing(plist->GetCurrent()));
                plist->GetNext();
            }

            // make sure we unload this so we will refresh it if we come back to
            // this screen.  
            GetModeler()->UnloadNameSpace(strNamespace);

            m_plistPaneEvents->SetList(plistEvents);
            UpdateButtons();
        }
        else
        {
            TRef<IMessageBox> pmsgBox = CreateMessageBox("Failed to load the event info from the Zone (file format error).  Please try again later.");
            GetWindow()->GetPopupContainer()->OpenPopup(pmsgBox, false);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Screen Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    Pane* GetPane()
    {
        return m_ppane;
    }


    //
    // download support
    //

    void OnFrame()
    {
        if (m_pSession)
        {
            // if the download has completed or an error occured...
            if (!m_pSession->ContinueDownload())
            {
                if (m_peventListingPending)
                {
                    DownloadEventListing(m_peventListingPending);
                    m_peventListingPending = NULL;
                }
                else
                {
                    // close the session
                    delete m_pSession;
                    m_pSession = NULL;
                }
            }
        }
    }

    void DownloadEventListing(EventListing* peventListing)
    {
        m_strWaitForFile = peventListing->GetFileList().GetEnd();


        // Generate the file list

        ZString* vstrSourceNames = new ZString[peventListing->GetFileList().GetCount()];
        PCC* vszFileList = new PCC[2*peventListing->GetFileList().GetCount() + 1];

        TList<ZString>::Iterator iter(peventListing->GetFileList());
        int i = 0;
        while (!iter.End())
        {
            vstrSourceNames[i] = trekClient.GetCfgInfo().strZoneEventDetailsURL + iter.Value();
            vszFileList[i*2] = vstrSourceNames[i];
            vszFileList[i*2 + 1] = PCC(iter.Value());

            // make sure we unload this so we can write to it.
            GetModeler()->UnloadNameSpace(iter.Value().LeftOf("."));

            i++;
            iter.Next();
        }
        vszFileList[i*2] = NULL;
        
        // start the download

        if (!m_pSession)
            m_pSession = CreateHTTPSession(this);

        m_pSession->InitiateDownload(vszFileList, trekClient.GetArtPath());


        // clean up

        delete [] vszFileList;
        delete [] vstrSourceNames;
    }

    void OnError(char * szErrorMessage) // on HTTP download error
    {
        // Errors are essentially ignored
        if (m_bLoadingEventList)
        {
            debugf("Error while trying to get the Zone Events list: %s", szErrorMessage);

            TRef<IMessageBox> pmessagebox = 
                CreateMessageBox("Failed to retrieve the Zone Events list.");
            GetWindow()->GetPopupContainer()->OpenPopup(pmessagebox);
        }
        else
        {
            debugf("Error while trying to get a Zone Events description file: %s", szErrorMessage);

            TRef<IMessageBox> pmessagebox = 
                CreateMessageBox("Failed to retrieve the event description.");
            GetWindow()->GetPopupContainer()->OpenPopup(pmessagebox);
        }

        // don't try to do a pending transaction after an error
        m_peventListingPending = NULL;
    }

    bool IsFileValid(char * szFileName)
    {
        ZFile file(szFileName);

        int n = file.GetLength(); // -1 means error
        if (n != -1 && n != 0) 
        {
            char * pData = new char[n+1];
            memcpy(pData, file.GetPointer(), n);
            pData[n] = 0;

            if (strstr(pData, "<html") != NULL
                || strstr(pData, "<HTML") != NULL
                || strstr(pData, "<Html") != NULL)
            {
                debugf("Found '<html' in %s; assumed failure.\n", szFileName);
                delete[] pData;
                return false;
            }
            else
            {
                delete[] pData;
                return true;
            }
        }
        else
        {
            debugf("File %s error while trying to load downloaded config file.\n", szFileName);
            return false;
        }
    }

    bool OnFileCompleted(char * szFileName)
    {
        debugf("Downloaded file: %s\n", szFileName);

        char szPath[MAX_PATH+20];
        strcpy(szPath, m_pSession->GetDownloadPath());
        strcat(szPath, szFileName);

        if (!IsFileValid(szPath))
        {
            debugf("Download failed.");

            if (m_nRetriesLeft)
            {
                // try again
                --m_nRetriesLeft;
                return false;
            }
            else
            {
                TRef<IMessageBox> pmsgBox = CreateMessageBox("Unable download event info from the Zone.  Please try again later.");
                GetWindow()->GetPopupContainer()->OpenPopup(pmsgBox, false);
            }
        }
        else if (m_bLoadingEventList)
        {
            m_bLoadingEventList = false;
            LoadEvents("zoneevents");
        }
        else
        {
            if (szFileName == m_strWaitForFile)
            {
                m_ppageDescription->SetTopic(m_pns, m_strWaitForFile.LeftOf(strlen(".mml")));
            }
        }

        // reset the retry counter
        m_nRetriesLeft = c_cRetries;

        return true; // true means don't retry download
    }

    void BeginZoneEventsDownload()
    {
        ZAssert(!m_pSession && !m_bLoadingEventList);
        if (m_pSession)
            return;

        m_bLoadingEventList = true;

        debugf("Beginning zoneevents download: %s.\n", PCC(trekClient.GetCfgInfo().strZoneEventsURL));

        m_pSession = CreateHTTPSession(this);

        static const char * szFileList[3] = {NULL, "zoneevents.mdl", NULL };
        szFileList[0] = PCC(trekClient.GetCfgInfo().strZoneEventsURL);

        m_pSession->InitiateDownload(szFileList, trekClient.GetArtPath());

        return;
    }

    void BeginEventListingDownload(EventListing* peventListing)
    {
        ZAssert(peventListing);

        if (m_pSession)
        {
            m_peventListingPending = peventListing;
        }
        else
        {
            DownloadEventListing(peventListing);
        }
    }
};


//////////////////////////////////////////////////////////////////////////////
//
// Constructor
//
//////////////////////////////////////////////////////////////////////////////

TRef<Screen> CreateZoneEventsScreen(Modeler* pmodeler)
{
    return new ZoneEventsScreen(pmodeler);
}
