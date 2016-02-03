#ifndef XAMINE_CLIENT_H
#define XAMINE_CLIENT_H

#include <vector>
#include <string>

class TestXamine {

private:
    std::vector<std::string> m_record;

private:
    static TestXamine* m_pInstance;

    TestXamine() : m_record() {}

    static TestXamine* getInstance();

    void log(std::string message);
    std::vector<std::string> getRecord() const { return m_record; }
};

int Xamine_CreateSharedMemory(int specbytes, volatile Xamine_shared **ptr);
int Xamine_DetachSharedMemory();

/* Start -- Starts up the Xamine process  */

int Xamine_Start();

/* Stop -- Stop the Xamine Process      */

int Xamine_Stop();

int Xamine_Alive();

void Xamine_GetMemoryName(char *namebuffer);

int Xamine_MapMemory(char *name, int specbytes,volatile Xamine_shared **ptr);

/*
** The functions below interface to Xamine's gate handling.
*/


int Xamine_EnterGate(int spec, int id, Xamine_gatetype type,
             char *name,
             int npts, Xamine_point *pts);
int Xamine_RemoveGate(int spec, int id, Xamine_gatetype ntype);

long Xamine_StartSearch(int spec, int *status);

int Xamine_NextGate(long context, int *id, Xamine_gatetype *type,
            int *npts, Xamine_point *pts);

void Xamine_EndSearch(long context);

int Xamine_ReadGate(int *spectrum, Xamine_gatetype *type , char *name,
            int *npts, Xamine_point *pts);

int Xamine_PollForGate(int timeout, int *spectrum,
               Xamine_gatetype *type,
               char *name,
               int *npts, Xamine_point *pts);


/*
** Routines below allow spectrum numbers to be allocated, deallocated
** and reserved.
*/

int Xamine_AllocateSpectrum();	    /* Get lowest numbered free spectrum. */

int Xamine_ReserveSpectrum(int spno); /* Reserve a particular spectrum. */

void Xamine_FreeSpectrum(int id);    /* Free a spectrum.   */

/*
**   Low level spectrum memory routines:
*/

void Xamine_ManageMemory();	/* Must be called first to init. */

caddr_t Xamine_AllocMemory(int size); /* Allocate Spectrum memory */

void    Xamine_FreeMemory(caddr_t loc);	/* Free memory chunk */

/*
** Low level routine to fill in a spectrum definition
*/

void Xamine_DescribeSpectrum(int spno, int xdim, int ydim,
                 char *title, caddr_t loc,
                 spec_type type);

/*
** High level routines to allocated, and describe spectra to the
** the memory management routines.
*/

caddr_t Xamine_Allocate1d(int *spno, int xdim, char *title, int word);
caddr_t Xamine_Allocate2d(int *spno, int xdim, int ydim, char *title,
              int type);

void Xamine_SetMap1d(int spno, float xmin, float xmax, spec_label xlabel);
void Xamine_SetMap2d(int spno, float xmin, float xmax, spec_label xlabel,
             float ymin, float ymax, spec_label ylabel);


/*
** The following functions control the user defined button boxes.
*/

                /* Create a button box ncol x nrow  */

int Xamine_DefineButtonBox(int ncol,int nrow);
int Xamine_DefineButton(int nrow, int ncol, void *button);
int Xamine_ModifyButton(int r, int c, void *button);

int Xamine_PollForEvent(int timeout, void *event);
int Xamine_ReadEvent(void *event);


int Xamine_EnableButton(int r, int c);
int Xamine_DisableButton(int r, int c);

int Xamine_DeleteButton(int r, int c);

int Xamine_DeleteButtonBox();
int Xamine_InquireButton(int r, int c,
             void  *ack);

int Xamine_EnterPeakMarker(int nSpectrum, int nId,
               char*              szName,
               float              fCentroid,
               float              fWidth);

int Xamine_EnterFitline(int nSpectrum,  int nId, char* szName,
                        int low, int high, char* szEvalScript);

void Xamine_setOverflow(unsigned nSpectrum, int axis, unsigned value);
void Xamine_setUnderflow(unsigned nSpectrum, int axis, unsigned value);
void Xamine_clearStatistics(unsigned nSpetrum);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif




#endif // CLIENT_H
