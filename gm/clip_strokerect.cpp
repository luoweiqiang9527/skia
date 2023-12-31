/*
 * Copyright 2011 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "gm/gm.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkColor.h"
#include "include/core/SkPaint.h"
#include "include/core/SkRect.h"
#include "include/core/SkSize.h"
#include "include/core/SkString.h"

class ClipStrokeRectGM : public skiagm::GM {
public:
    ClipStrokeRectGM() {

    }

protected:
    SkString getName() const override { return SkString("clip_strokerect"); }

    SkISize getISize() override { return SkISize::Make(200, 400); }

    void onDraw(SkCanvas* canvas) override {
        SkPaint p;
        p.setColor(SK_ColorRED);
        p.setAntiAlias(true);
        p.setStyle(SkPaint::kStroke_Style);
        p.setStrokeWidth(22);

        SkRect r = SkRect::MakeXYWH(20, 20, 100, 100);
        // setting the height of this to 19 causes failure
        SkRect rect = SkRect::MakeXYWH(20, 0, 100, 20);

        canvas->save();
        canvas->clipRect(rect, true);
        canvas->drawRect(r, p);
        canvas->restore();

        p.setColor(SK_ColorBLUE);
        p.setStrokeWidth(2);
        canvas->drawRect(rect, p);

        p.setColor(SK_ColorRED);
        p.setAntiAlias(true);
        p.setStyle(SkPaint::kStroke_Style);
        p.setStrokeWidth(22);

        SkRect r2 = SkRect::MakeXYWH(20, 140, 100, 100);
        // setting the height of this to 19 causes failure
        SkRect rect2 = SkRect::MakeXYWH(20, 120, 100, 19);

        canvas->save();
        canvas->clipRect(rect2, true);
        canvas->drawRect(r2, p);
        canvas->restore();

        p.setColor(SK_ColorBLUE);
        p.setStrokeWidth(2);
        canvas->drawRect(rect2, p);
    }

private:
    using INHERITED = skiagm::GM;
};

DEF_GM(return new ClipStrokeRectGM;)
