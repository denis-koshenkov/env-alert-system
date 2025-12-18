#include "central_event_queue.h"

int main(void)
{
    central_event_queue_init();
    central_event_queue_submit_init_event();
    /* Main thread has higher priority than the central event queue thread, so the main thread will reach this point and
     * terminate normally. Now, the central event queue thread will handle the init event that was submitted above. */
    return 0;
}
