#ifndef PLAYLISTDAO_H
#define PLAYLISTDAO_H

#include <QHash>
#include <QObject>
#include <QSqlDatabase>
#include <QSet>

#include "library/dao/dao.h"
#include "util.h"

#define PLAYLIST_TABLE "Playlists"
#define PLAYLIST_TRACKS_TABLE "PlaylistTracks"

const QString PLAYLISTTABLE_ID = "id";
const QString PLAYLISTTABLE_NAME = "name";
const QString PLAYLISTTABLE_POSITION = "position";
const QString PLAYLISTTABLE_HIDDEN = "hidden";
const QString PLAYLISTTABLE_DATECREATED = "date_created";
const QString PLAYLISTTABLE_DATEMODIFIED = "date_modified";

const QString PLAYLISTTRACKSTABLE_TRACKID = "track_id";
const QString PLAYLISTTRACKSTABLE_POSITION = "position";
const QString PLAYLISTTRACKSTABLE_PLAYLISTID = "playlist_id";
const QString PLAYLISTTRACKSTABLE_LOCATION = "location";
const QString PLAYLISTTRACKSTABLE_ARTIST = "artist";
const QString PLAYLISTTRACKSTABLE_TITLE = "title";
const QString PLAYLISTTRACKSTABLE_DATETIMEADDED = "pl_datetime_added";

class PlaylistDAO : public QObject, public virtual DAO {
    Q_OBJECT
  public:
    enum HiddenType {
        PLHT_NOT_HIDDEN = 0,
        PLHT_AUTO_DJ = 1,
        PLHT_SET_LOG = 2,
        PLHT_UNKNOWN = -1
    };

    PlaylistDAO(QSqlDatabase& database);
    virtual ~PlaylistDAO();

    void initialize();
    void setDatabase(QSqlDatabase& database) { m_database = database; }
    // Create a playlist, fails with -1 if already exists
    int createPlaylist(const QString& name, const HiddenType type = PLHT_NOT_HIDDEN);
    // Create a playlist, appends "(n)" if already exists, name becomes the new name
    int createUniquePlaylist(QString* pName, const HiddenType type = PLHT_NOT_HIDDEN);
    // Delete a playlist
    void deletePlaylist(const int playlistId);
    // Rename a playlist
    void renamePlaylist(const int playlistId, const QString& newName);
    // Lock or unlock a playlist
    bool setPlaylistLocked(const int playlistId, const bool locked);
    // Find out the state of a playlist lock
    bool isPlaylistLocked(const int playlistId) const;
    // Append a list of tracks to a playlist
    bool appendTracksToPlaylist(const QList<int>& trackIds, const int playlistId);
    // Append a track to a playlist
    bool appendTrackToPlaylist(const int trackId, const int playlistId);
    // Find out how many playlists exist.
    unsigned int playlistCount() const;
    // Find out the name of the playlist at the given Id
    QString getPlaylistName(const int playlistId) const;
    // Get the playlist id by its name
    int getPlaylistIdFromName(const QString& name) const;
    // Get the id of the playlist at index. Note that the index is the natural
    // position in the database table, not the display order position column
    // stored in the database.
    int getPlaylistId(const int index) const;
    QList<int> getTrackIds(const int playlistId) const;
    // Returns true if the playlist with playlistId is hidden
    bool isHidden(const int playlistId) const;
    // Returns the HiddenType of playlistId
    HiddenType getHiddenType(const int playlistId) const;
    // Returns the maximum position of the given playlist
    int getMaxPosition(const int playlistId) const;
    // Remove a track from all playlists
    void removeTracksFromPlaylists(const QList<int>& ids);
    // Remove a track from a playlist
    void removeTrackFromPlaylist(const int playlistId, const int position);
    void removeTracksFromPlaylist(const int playlistId, QList<int>& positions);
    // Insert a track into a specific position in a playlist
    bool insertTrackIntoPlaylist(int trackId, int playlistId, int position);
    // Inserts a list of tracks into playlist
    int insertTracksIntoPlaylist(const QList<int>& trackIds, const int playlistId, int position);
    // Add a playlist to the Auto-DJ Queue
    void addPlaylistToAutoDJQueue(const int playlistId, const bool bTop);
    // Add a list of tracks to the Auto-DJ Queue
    void addTracksToAutoDJQueue(const QList<int>& trackIds, const bool bTop);
    // Get the preceding playlist of currentPlaylistId with the HiddenType
    // hidden. Returns -1 if no such playlist exists.
    int getPreviousPlaylist(const int currentPlaylistId, HiddenType hidden) const;
    // Append all the tracks in the source playlist to the target playlist.
    bool copyPlaylistTracks(const int sourcePlaylistID, const int targetPlaylistID);
    // Returns the number of tracks in the given playlist.
    int tracksInPlaylist(const int playlistId) const;
    // moved Track to a new position
    void moveTrack(const int playlistId,
            const int oldPosition, const int newPosition);
    // shuffles all tracks in the position List
    void shuffleTracks(const int playlistId, const QList<int>& positions, const QHash<int,int>& allIds);
    bool isTrackInPlaylist(const int trackId, const int playlistId) const;

    void getPlaylistsTrackIsIn(const int trackId, QSet<int>* playlistSet) const;

  signals:
    void added(int playlistId);
    void deleted(int playlistId);
    void changed(int playlistId);
    void trackAdded(int playlistId, int trackId, int position);
    void trackRemoved(int playlistId, int trackId, int position);
    void renamed(int playlistId, QString a_strName);
    void lockChanged(int playlistId);

  private:
    void removeTracksFromPlaylistsInner(const QStringList& idList);
    void searchForDuplicateTrack(const int fromPosition,
                                 const int toPosition,
                                 const int trackID,
                                 const int excludePosition,
                                 const int otherTrackPosition,
                                 const QHash<int,int>* pTrackPositionIds,
                                 int* pTrackDistance);
    void populatePlaylistMembershipCache();

    QSqlDatabase& m_database;
    QMultiHash<int,int> m_playlistsTrackIsIn;
    DISALLOW_COPY_AND_ASSIGN(PlaylistDAO);
};

#endif //PLAYLISTDAO_H
