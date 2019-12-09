module app;

import dlangui;
import plot2d;

mixin APP_ENTRY_POINT;

auto ct()
{
    import std.datetime : Clock;
    return Clock.currStdTime / 1e7;
}

class PlotWidget : CanvasWidget
{
    Plot plot;
    DlangUICtx ctx;

    this()
    {
        plot = new Plot;
        plot.settings.minGridStep.x = 80;
        plot.settings.minGridStep.y = 80;
        ctx = new DlangUICtx;

        auto mf(float i) { return sin(i*sin(i+ct()*0.5)*PI*2); }
        auto tre = iota(-2, 2.02, 0.02)
            .map!(i=>TreStat(i, mf(i) + 0.4 + sin(i*PI*2+ct*3) * 0.3,
                            mf(i), mf(i) - 0.4 - sin(i*PI*2+ct*5) * 0.3));

        plot.add(new TreChart(
            PColor(0,0.5,0,0.8),

            PColor(1,1,0,.3),
            PColor(1,1,0,.2),

            PColor(0,1,1,.3),
            PColor(0,1,1,.2),
            (ref Appender!(TreStat[]) buf) { buf.put(tre.save); }
        ));

        plot.settings.viewport = Viewport(DimSeg(-2,2), DimSeg(-1, 2));
        plot.settings.autoFit = false;
        plot.settings.padding = Border(0);
    }

    override void doDraw(DrawBuf buf, Rect rc)
    {
        buf.fillRect(rc, 0xaaaaaa);
        auto _ = ctx.set(buf);
        plot.updateCharts();
        plot.draw(ctx, PPoint(this.width, this.height));
    }
}

extern (C) int UIAppMain(string[] args)
{
    Window window = Platform.instance.createWindow(
        "example", null, WindowFlag.Resizable, 1000, 600);

    window.mainWidget = new PlotWidget();

    window.show();

    return Platform.instance.enterMessageLoop();
}