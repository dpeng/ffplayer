/**
* \file
* \author Trevor Fountain
* \author Johannes Buchner
* \author Erik Garrison
* \date 2010-2014
* \copyright BSD 3-Clause
*
* progressbar -- a C class (by convention) for displaying progress
* on the command line (to stdout).
*/
#pragma warning (disable : 4244)
#pragma warning (disable : 4267)
#pragma warning (disable : 4996)

#include "termcap.h"  /* tgetent, tgetnum */
#include <assert.h>
#include <limits.h>
#include "progressbar.h"

///  How wide we assume the screen is if termcap fails.
enum { DEFAULT_SCREEN_WIDTH = 80 };
/// The smallest that the bar can ever be (not including borders)
enum { MINIMUM_BAR_WIDTH = 10 };
/// The format in which the estimated remaining time will be reported
static const char *const ETA_FORMAT = "%02d:%02d";
/// The maximum number of characters that the ETA_FORMAT can ever yield
enum { ETA_FORMAT_LENGTH  = 13 };
/// Amount of screen width taken up by whitespace (i.e. whitespace between label/bar/ETA components)
enum { WHITESPACE_LENGTH = 2 };
/// The amount of width taken up by the border of the bar component.
enum { BAR_BORDER_WIDTH = 2 };

/// Models a duration of time broken into hour/minute/second components. The number of seconds should be less than the
/// number of seconds in one minute, and the number of minutes should be less than the number of minutes in one hour.
typedef struct {
  int hours;
  int minutes;
  int seconds;
} progressbar_time_components;

static void progressbar_draw(progressbar *bar, char* otherinfo);

/**
* Create a new progress bar with the specified label, max number of steps, and format string.
* Note that `format` must be exactly three characters long, e.g. "<->" to render a progress
* bar like "<---------->". Returns NULL if there isn't enough memory to allocate a progressbar
*/
progressbar *progressbar_new_with_format(const char *label, unsigned long max, const char *format)
{
  progressbar *new = malloc(sizeof(progressbar));
  if(new == NULL) {
    return NULL;
  }
    
  new->max = max;
  new->curPos = 0;
  new->otherInfoLen = 0;
  new->start = time(NULL);
  assert(3 == strlen(format) && "format must be 3 characters in length");
  new->format.begin = format[0];
  new->format.fill = format[1];
  new->format.end = format[2];
  new->currentTime = 0;
  new->leftTime = 0;

  progressbar_update_label(new, label);
  progressbar_draw(new, NULL);

  return new;
}

/**
* Create a new progress bar with the specified label and max number of steps.
*/
progressbar *progressbar_new(const char *label, unsigned long max)
{
    return progressbar_new_with_format(label, max, "|=|");
}

void progressbar_update_label(progressbar *bar, const char *label)
{
  bar->label = label;
}

/**
* Delete an existing progress bar.
*/
void progressbar_free(progressbar *bar)
{
  free(bar);
}

/**
* Increment an existing progressbar by `value` steps.
*/
void progressbar_update(progressbar *bar, unsigned long pos, char* otherinfo)
{
  bar->curPos = pos;
  progressbar_draw(bar, otherinfo);
}

static void progressbar_write_char(FILE *file, const int ch, const size_t times) {
  size_t i;
  for (i = 0; i < times; ++i) {
    fputc(ch, file);
  }
}

void progressbar_clear(progressbar * bar)
{
	if(bar) progressbar_write_char(stdout, ' ', bar->max);
	fputc('\r', stdout);
}

static int progressbar_max(int x, int y) {
  return x > y ? x : y;
}

static unsigned int get_screen_width(void) {
  char termbuf[2048];
  if (tgetent(termbuf, getenv("TERM")) >= 0) {
    return tgetnum("co") /* -2 */;
  } else {
    return DEFAULT_SCREEN_WIDTH;
  }
}

static int progressbar_bar_width(int screen_width, int label_length) {
  return progressbar_max(MINIMUM_BAR_WIDTH, screen_width - label_length - ETA_FORMAT_LENGTH - WHITESPACE_LENGTH);
}

static int progressbar_label_width(int screen_width, int label_length, int bar_width) {
  int eta_width = ETA_FORMAT_LENGTH;

  // If the progressbar is too wide to fit on the screen, we must sacrifice the label.
  if (label_length + 1 + bar_width + 1 + ETA_FORMAT_LENGTH > screen_width) {
    return progressbar_max(0, screen_width - bar_width - eta_width - WHITESPACE_LENGTH);
  } else {
    return label_length;
  }
}

static int progressbar_remaining_seconds(const progressbar* bar) {
  double offset = difftime(time(NULL), bar->start);
  if (bar->curPos > 0 && offset > 0) {
    return (offset / (double) bar->curPos) * (bar->max - bar->curPos);
  } else {
    return 0;
  }
}

static progressbar_time_components progressbar_calc_time_components(int seconds) {
  int hours = seconds / 3600;
  seconds -= hours * 3600;
  int minutes = seconds / 60;
  seconds -= minutes * 60;

  progressbar_time_components components = {hours, minutes, seconds};
  return components;
}

static void progressbar_draw(progressbar *bar, char* otherinfo)
{
  int screen_width = get_screen_width();
  int label_length = strlen(bar->label);
  int bar_width = progressbar_bar_width(screen_width, label_length);
  int label_width = progressbar_label_width(screen_width, label_length, bar_width);

  int progressbar_completed = (bar->curPos >= bar->max);
  int bar_piece_count = bar_width - BAR_BORDER_WIDTH;
  int bar_piece_current = (progressbar_completed)
                          ? bar_piece_count
                          : bar_piece_count * ((double) bar->curPos / bar->max);

  progressbar_time_components eta = (progressbar_completed)
		                            ? progressbar_calc_time_components(difftime(time(NULL), bar->start))
		                            : progressbar_calc_time_components(bar->currentTime);

  progressbar_time_components etd = (progressbar_completed)
	  ? progressbar_calc_time_components(difftime(time(NULL), bar->start))
	  : progressbar_calc_time_components(bar->leftTime);

  // Draw the ETA
  fprintf(stdout, ETA_FORMAT, eta.minutes, eta.seconds);

  // Draw the progressbar
  fputc(bar->format.begin, stdout);
  progressbar_write_char(stdout, bar->format.fill, bar_piece_current);
  progressbar_write_char(stdout, ' ', bar_piece_count - bar_piece_current);
  fputc(bar->format.end, stdout);
  fputc(' ', stdout);

  /*remove the lable by dpeng
  if (label_width == 0) {
    // The label would usually have a trailing space, but in the case that we don't print
    // a label, the bar can use that space instead.
    bar_width += 1;
  } else {
    // Draw the label
    fwrite(bar->label, 1, label_width, stdout);
	fputc('\r', stdout);
  }*/

  // Draw the ETD
  fprintf(stdout, ETA_FORMAT, etd.minutes, etd.seconds);
  if (otherinfo) { 
	  fprintf(stdout, " | "); 
	  fprintf(stdout, otherinfo); 
	  if(strlen(otherinfo) < bar->otherInfoLen)
		progressbar_write_char(stdout, ' ', bar->otherInfoLen - strlen(otherinfo));
	  bar->otherInfoLen = strlen(otherinfo);
  }
  fputc('\r', stdout);
  }

/**
* Finish a progressbar, indicating 100% completion, and free it.
*/
void progressbar_finish(progressbar *bar)
{
  // Make sure we fill the progressbar so things look complete.
  //progressbar_draw(bar);

  // Print a newline, so that future outputs to stdout look prettier
  fprintf(stdout, "\n");

  // We've finished with this progressbar, so go ahead and free it.
  progressbar_free(bar);
}
