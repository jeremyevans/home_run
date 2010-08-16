require File.expand_path('../../spec_helper', __FILE__)

describe "Date#ajd" do
  it "should be able to determine the Astronomical Julian day for a date" do
    Date.civil(2007, 1, 17).ajd.should == 2454118
  end
end

describe "Date#amjd" do
  it "should be able to determine the Astronomical Modified Julian day for a date" do
    Date.civil(2007, 1, 17).amjd.should == 54117
  end
end

describe "Date#day_fraction" do
  it "should be able to determine the day fraction for a date" do
    Date.civil(2007, 1, 17).day_fraction.should == 0
  end
end

describe "Date#jd" do
  it "should be able to determine the julian date for a date" do
    Date.civil(2007,  1, 17).jd.should == 2454118
    Date.civil(2008, 10, 28).jd.should == 2454768
    Date.commercial(2008, 1, 1).jd.should == 2454466
    Date.commercial(2008, 52, 1).jd.should == 2454823
    Date.jd(2454782).jd.should == 2454782
    Date.jd(2454832).jd.should == 2454832
    Date.ordinal(2008, 1).jd.should == 2454467
    Date.ordinal(2008, 359).jd.should == 2454825
    Date.ordinal(2008, 366).jd.should == 2454832
  end 
end

describe "Date#mjd" do
  it "should be able to determine the Modified Julian day for a date" do
    Date.civil(2007, 1, 17).mjd.should == 54117
  end 
end

describe "Date#ld" do
  it "should be able to determine the Modified Julian day for a date" do
    Date.civil(2007, 1, 17).ld.should == 154958
  end 
end

describe "Date#year" do
  it "should be able to determine the year for a date" do
    Date.civil(2007, 1, 17).year.should == 2007
    Date.civil(2008, 1, 17).year.should == 2008
    Date.commercial(2008, 1, 1).year.should == 2007
    Date.commercial(2008, 52, 1).year.should == 2008
    Date.jd(2454782).year.should == 2008
    Date.jd(2454833).year.should == 2009
    Date.ordinal(2008, 1).year.should == 2008
    Date.ordinal(2008, 170).year.should == 2008
    Date.ordinal(2008, 366).year.should == 2008
  end 
end

describe "Date#yday" do
  it "should be able to determine the year for a date" do
    Date.civil(2007,  1, 17).yday.should == 17
    Date.civil(2008, 10, 28).yday.should == 302
    Date.commercial(2008, 1, 1).yday.should == 365
    Date.commercial(2008, 52, 1).yday.should == 357
    Date.jd(2454782).yday.should == 316
    Date.jd(2454832).yday.should == 366
    Date.ordinal(2008, 1).yday.should == 1
    Date.ordinal(2008, 170).yday.should == 170
    Date.ordinal(2008, 366).yday.should == 366
  end 
end

describe "Date#mon and #month" do
  it "should be able to determine the month for a date" do
    Date.civil(2007,  1, 17).mon.should == 1
    Date.civil(2008, 10, 28).mon.should == 10
    Date.civil(2007,  1, 17).month.should == 1
    Date.civil(2008, 10, 28).month.should == 10
    Date.commercial(2008, 1, 1).mon.should == 12
    Date.commercial(2008, 52, 1).mon.should == 12
    Date.jd(2454782).mon.should == 11
    Date.jd(2454832).mon.should == 12
    Date.ordinal(2008, 1).mon.should == 1
    Date.ordinal(2008, 170).mon.should == 6
    Date.ordinal(2008, 366).mon.should == 12
  end 
end

describe "Date#mday and #day" do
  it "should be able to determine the day of the month for a date" do
    Date.civil(2007,  1, 17).mday.should == 17
    Date.civil(2008, 10, 28).mday.should == 28
    Date.civil(2007,  1, 17).day.should == 17
    Date.civil(2008, 10, 28).day.should == 28
    Date.commercial(2008, 1, 1).day.should == 31
    Date.commercial(2008, 52, 1).day.should == 22
    Date.jd(2454782).day.should == 11
    Date.jd(2454832).day.should == 31
    Date.ordinal(2008, 1).day.should == 1
    Date.ordinal(2008, 359).day.should == 24
    Date.ordinal(2008, 366).day.should == 31
  end
end

describe "Date#wday" do
  it "should be able to determine the week day for a date" do
    Date.civil(2007,  1, 17).wday.should == 3
    Date.civil(2008, 10, 26).wday.should == 0
    Date.commercial(2008, 1, 1).wday.should == 1
    Date.commercial(2008, 52, 1).wday.should == 1
    Date.jd(2454782).wday.should == 2
    Date.jd(2454832).wday.should == 3
    Date.ordinal(2008, 1).wday.should == 2
    Date.ordinal(2008, 170).wday.should == 3
    Date.ordinal(2008, 366).wday.should == 3
  end
end

describe "Date#cwyear" do
  it "should be able to determine the commercial year for a date" do
    Date.civil(2007,  1, 17).cwyear.should == 2007
    Date.civil(2008, 10, 28).cwyear.should == 2008
    Date.civil(2007, 12, 31).cwyear.should == 2008
    Date.civil(2010,  1,  1).cwyear.should == 2009
    Date.commercial(2008, 1, 1).cwyear.should == 2008
    Date.commercial(2008, 52, 1).cwyear.should == 2008
    Date.jd(2454782).cwyear.should == 2008
    Date.jd(2454832).cwyear.should == 2009
    Date.ordinal(2008, 1).cwyear.should == 2008
    Date.ordinal(2008, 359).cwyear.should == 2008
    Date.ordinal(2008, 366).cwyear.should == 2009
  end
end

describe "Date#cweek" do
  it "should be able to determine the commercial week for a date" do
    Date.civil(2007,  1, 17).cweek.should == 3
    Date.civil(2008, 10, 28).cweek.should == 44
    Date.civil(2007, 12, 31).cweek.should == 1
    Date.civil(2010,  1,  1).cweek.should == 53
    Date.commercial(2008, 1, 1).cweek.should == 1
    Date.commercial(2008, 10, 5).cweek.should == 10
    Date.jd(2454782).cweek.should == 46
    Date.jd(2454789).cweek.should == 47
    Date.ordinal(2008, 1).cweek.should == 1
    Date.ordinal(2008, 359).cweek.should == 52
    Date.ordinal(2008, 366).cweek.should == 1
  end
end

describe "Date#cwday" do
  it "should be able to determine the commercial week day for a date" do
    Date.civil(2007,  1, 17).cwday.should == 3
    Date.civil(2008, 10, 26).cwday.should == 7
    Date.commercial(2008, 1, 1).cwday.should == 1
    Date.commercial(2008, 10, 5).cwday.should == 5
    Date.jd(2454782).cwday.should == 2
    Date.jd(2454786).cwday.should == 6
    Date.ordinal(2008, 1).cwday.should == 2
    Date.ordinal(2008, 317).cwday.should == 3
  end  
end

describe "Date#start" do
  it "#start should be less than a low number" do
    Date.civil(2008, 10, 11).start.should < -(2**30)
  end
  
  ruby_version_is "" ... "1.9" do
    it "#sg should be less than a low number" do
      Date.civil(2008, 10, 11).sg.should < -(2**30)
    end
  end
end
