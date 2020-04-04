#ifndef panel_h
#define panel_h

#include "buttonmanager.h"
#include "encodermanager.h"
#include "nhd_0420d3z.h"
#include "ventsettings.h"
#include "Encoder.h"

class Panel {
  public:

    Panel(NhdDisplay* disp_ptr, Encoder* encoder_ptr, ButtonManager* em_button_ptr, ButtonManager* stop_button_ptr, VentSettings* vs_ptr);

    virtual void start() = 0;
    virtual Panel* update() = 0;

  protected:

    // Pointers for settings and input.
    NhdDisplay* _disp_ptr;
    Encoder* _encoder_ptr;
    ButtonManager* _em_button_ptr;
    ButtonManager* _stop_button_ptr;
    VentSettings* _vs_ptr;


};

class SplashPanel : public Panel {
  public:
    SplashPanel(NhdDisplay* disp_ptr, Encoder* encoder_ptr, ButtonManager* em_button_ptr, ButtonManager* stop_button_ptr, VentSettings* vs_ptr, String* text, int display_time, Panel** next_ptr);

    void start();
    Panel* update();

  private:
    // Used for basic prompts, displays the panel for this number of ms.
    int _display_time;
    String* _text;

    Panel** _next_d_ptr;
    Panel* _next_ptr;

};

class EditPanel : public Panel {
  public:
    EditPanel(NhdDisplay* disp_ptr, Encoder* encoder_ptr, ButtonManager* em_button_ptr, ButtonManager* stop_button_ptr, VentSettings* vs_ptr, String top_text, Panel** run_panel_ptr, Panel** stop_panel_ptr);

    void start();
    Panel* update();

  private:

    EncoderManager* _em_ptr;

    String _top_text;
    
    String _tv_text = "TV = ";
    int _tv_text_length = 5;
    String _tv_units = " mL";

    String _rr_text = "RR = ";
    int _rr_text_length = 5;
    String _rr_units = " BPM";

    String _i_e_text = "I:E = ";
    int _i_e_text_length = 8;

    Panel** _run_panel_d_ptr;
    Panel** _stop_panel_d_ptr;
    Panel* _run_panel_ptr;
    Panel* _stop_panel_ptr;

    int _min_respiration_rate = 10;
    int _max_respiration_rate = 30;
    int _delta_respiration_rate = 1;

    int _min_exhale = 1;
    int _max_exhale = 5;
    int _delta_exhale = 1;

    int _min_tidal_volume = 300;
    int _max_tidal_volume = 650;
    int _delta_tidal_volume = 50;

    int _row = 0;
    bool _edit = false;
    int _old_selection = 0;
    bool _made_change = false;

};

class RunningPanel : public Panel {
  public:
    RunningPanel(NhdDisplay* disp_ptr, Encoder* encoder_ptr, ButtonManager* em_button_ptr, ButtonManager* stop_button_ptr, VentSettings* vs_ptr, Panel** apply_panel_ptr, Panel** stop_panel_ptr);

    void start();
    Panel* update();

    String formatTime();

  private:

    Panel** _apply_panel_d_ptr;
    Panel** _stop_panel_d_ptr;
    Panel* _apply_panel_ptr;
    Panel* _stop_panel_ptr;

    String _top_text = "Run Time: ";
    int _text_length_to_time = 10;

    String _tv_text = "TV = ";
    String _tv_units = " mL";

    String _rr_text = "RR = ";
    String _rr_units = " BPM";

    String _i_e_text = "I:E = ";

};

class PausePanel : public Panel {
  public:
    PausePanel(NhdDisplay* disp_ptr, Encoder* encoder_ptr, ButtonManager* em_button_ptr, ButtonManager* stop_button_ptr, VentSettings* vs_ptr, Panel** apply_panel_ptr, Panel** run_panel_ptr);

    void start();
    Panel* update();

  private:

    EncoderManager* _em_ptr;

    Panel** _apply_panel_d_ptr;
    Panel** _run_panel_d_ptr;
    Panel* _apply_panel_ptr;
    Panel* _run_panel_ptr;
    // Looks like " T00:11:22 Run/Edit?"

    String _top_before_time = "T";
    String _top_after_time = " Run/Edit?";
    int _text_length_to_run = 11;
    int _text_length_to_edit = 15;

    String _tv_text = "TV = ";
    String _tv_units = " mL";

    String _rr_text = "RR = ";
    String _rr_units = " BPM";

    String _i_e_text = "I:E = ";

    int _selection = 0;
};
#endif
