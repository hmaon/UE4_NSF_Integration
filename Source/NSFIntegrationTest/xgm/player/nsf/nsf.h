#ifndef _NSF_H_
#define _NSF_H_
#include "../soundData.h"

#define NSF_MAX_PATH 512

namespace xgm
{
  struct NSFE_Entry
  {
    const char* tlbl;
    const char* taut;
    INT32 time;
    INT32 fade;
    bool psfx;
  };
  const unsigned int NSFE_ENTRIES = 256;
  const unsigned int NSFE_MIXES = 8;
  const INT16 NSFE_MIXE_DEFAULT = 32767;

  /**
   * NSFファイル型
   */
  class NSF : SoundDataMSP
  {
  public:
    char magic[5];
    UINT8 version;
    UINT8 songs;
    UINT8 total_songs;
    UINT8 start;
    UINT16 load_address;
    UINT16 init_address;
    UINT16 play_address;
    char filename[NSF_MAX_PATH];
    char print_title[256+64]; // margin 64 chars.
    char title_nsf[32];
    char artist_nsf[32];
    char copyright_nsf[32];
    char* title;
    char* artist;
    char* copyright;
    char* ripper; // NSFe only
    char* text; // NSFe only
    UINT32 text_len; // NSFe only
    UINT16 speed_ntsc;
    UINT8 bankswitch[8];
    UINT16 speed_pal;
    UINT8 pal_ntsc;
    UINT8 soundchip;
    bool use_vrc7;
    bool use_vrc6;
    bool use_fds;
    bool use_fme7;
    bool use_mmc5;
    bool use_n106;
    UINT8 extra[4];
    UINT8 *body;
    int bodysize;
    UINT8* nsfe_image;
    UINT8* nsfe_plst;
    int nsfe_plst_size;
    NSFE_Entry nsfe_entry[NSFE_ENTRIES];
    INT16 nsfe_mixe[NSFE_MIXES];
    int vrc7_type;
    const UINT8* vrc7_patches;
    UINT16 speed_dendy;
    UINT8 regn;
    int regn_pref;
    UINT8 nsf2_bits;

    /** 現在選択中の曲番号 */
    int song;
    /** プレイリストから読んだデータならtrue */
    bool playlist_mode;
    /** 演奏時間 */
    int time_in_ms, default_playtime;
    /** ループ時間 */
    int loop_in_ms;
    /** フェード時間 */
    int fade_in_ms, default_fadetime;
    /** ループ回数 */
    int loop_num, default_loopnum;
    /** 演奏時間が不明の時true（デフォルトの演奏時間を使用）*/
    bool playtime_unknown;
    bool title_unknown;

      NSF ();
     ~NSF ();
    /**
     * NSFファイルの読み込み
     *
     * @param image バイナリイメージへのポインタ
     * @param size  イメージのサイズ
     * @return 成功時 true 失敗時 false
     */

    // loads file (playlist or NSF or NSFe)
    bool LoadFile (const char *fn);

    // loads NSF (or NSFe via LoadNSFe)
    bool Load (UINT8 * image, UINT32 size);

    // loads NSFe, or NSFe suffix for NSF2
    bool LoadNSFe(UINT8* image, UINT32 size, bool nsf2);

    // returns descriptive error of last Load (English only)
    const char* LoadError();

    void DebugOut ();
    /**
     * タイトル文字列の取得
     *
     * @return タイトル文字列 (作曲者 - タイトル)
     */
    char *GetTitleString (const char *format=NULL, int song=-1);
    char *GetPlaylistString (const char *format, bool b);
    int GetLength ();
    void SetTitleString (char *);
    void SetDefaults (int playtime, int fadetime, int loopnum);
    void SetLength (int time_in_ms);
    void SetSong (int);
    int GetSong ();
    int GetSongNum();

    int GetPlayTime ();
    int GetLoopTime ();
    int GetFadeTime ();
    int GetLoopNum ();

    bool UseNSFePlaytime();
  };

}                               // namespace 
#endif
