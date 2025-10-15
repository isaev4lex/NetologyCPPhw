#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>

class Stopwatch : public QObject
{
    Q_OBJECT
public:
    explicit Stopwatch(QObject* parent = nullptr);

    bool   isRunning() const {
        return running_;
    }

    qint64 elapsedMs() const;

    int    lapCount() const {
        return lapIndex_;
    }

public slots:
    void start();
    void stop();
    void reset();
    void setUpdateIntervalMs(int ms);
    void tick();
    void markLap();

signals:
    void elapsedChanged(qint64 ms);
    void runningChanged(bool running);
    void lapRecorded(int index, qint64 lapMs);
    void resetDone();

private:
    qint64 nowTotalMs() const;
    static qint64 clampNonNeg(qint64 v) {
        return v < 0 ? 0 : v;
    }

    QTimer        timer_;
    QElapsedTimer coarse_;
    bool          running_ = false;

    qint64 baseAccumMs_      = 0;
    qint64 lastLapMarkMs_    = 0;
    int    lapIndex_         = 0;
};


#endif // STOPWATCH_H
