#ifndef COMMMGR_H
#define COMMMGR_H

#include <QTimer>

#include "CommMgrInterface.h"

#include "JamCtrlMgr.h"

class CommMgr : public CommMgrInterface
{
    Q_OBJECT

  public:
    using DataIn = JamCtrlMgr::DataIn;
    using IntegerNumType = JamCtrlMgr::IntegerNumType;
    using FloatNumType = JamCtrlMgr::FloatNumType;

    explicit CommMgr();
    virtual ~CommMgr();

    void start();
    void stop();

  public slots:
    void onFio2Changed(int value) override;
    void onTidalVolChanged(int value) override;
    void onRespRateChanged(int value) override;
    void onIeRatioChanged(int value) override;
    void onPeepChanged(int value) override;

    void onValveAO2OpenChanged(bool isOpen) override;
    void onValveBAirOpenChanged(bool isOpen) override;
    void onValveCInhaleOpenChanged(bool isOpen) override;
    void onValveDExhaleOpenChanged(bool isOpen) override;

  private:
    void onTimeout();

    JamCtrlMgr* m_jamCtrlMgr; // sim setup for now...
    QTimer m_timer;          // Timer for polling communication
};
#endif /* COMMMGR_H */
