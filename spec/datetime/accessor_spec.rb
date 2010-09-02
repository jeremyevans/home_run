require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime#day_fraction" do
  it "should be able to determine the fraction of the day" do
    DateTime.new(2008, 1, 1).day_fraction.should == 0.0
    DateTime.new(2008, 1, 1, 12).day_fraction.should == 0.5
    DateTime.new(2008, 1, 1, 6).day_fraction.should == 0.25
  end 
end

describe "DateTime#hour" do
  it "should be able to determine the hour of the day" do
    DateTime.jd(2007, 1).hour.should == 1
  end
end

describe "DateTime#min" do
  it "should be able to determine the minute of the day" do
    DateTime.jd(2007, 1, 2).min.should == 2
  end
end

describe "DateTime#offset" do
  it "should be able to determine the offset of the day from UTC" do
    DateTime.jd(2007, 1, 2, 3, 6/24.0).offset.should == 6/24.0
    DateTime.parse('2008-01-01 00:00:00+12:00').offset.should == 0.5
  end
  
  ruby_version_is "" ... "1.9" do
    it "#of should be the offset as a float fraction of the day" do
      DateTime.parse('2008-01-01 00:00:00+12:00').of.should == 0.5
    end
  end
end

describe "DateTime#sec" do
  it "should be able to determine the second of the day" do
    DateTime.jd(2007, 1, 2, 3).sec.should == 3
  end 
end

describe "DateTime#sec_fraction" do
  ruby_version_is "" ... "1.9" do
    it "should be the fraction of a second as a fraction of the day" do
      DateTime.new(2008, 1, 1).sec_fraction.should == 0.0
      DateTime.parse('12:13:15.678900').sec_fraction.should be_close(7.85763888888889e-06, 0.0000000000001)
    end 
  end

  ruby_version_is "1.9" do
    it "should be the fraction of a second" do
      DateTime.new(2008, 1, 1).sec_fraction.should == 0.0
      DateTime.parse('12:13:15.678900').sec_fraction.should be_close(0.6789, 0.0000000000001)
    end 
  end
end

describe "DateTime#zone" do
  it "should give the offset as a string" do
    DateTime.jd(0).zone.should == '+00:00'
    DateTime.jd(2007, 0, 0, 0, -1/24.0).zone.should == '-01:00'
  end 
end

describe "DateTime#cwyear" do
  it "should be able to determine the commercial year for a date" do
    DateTime.civil(2007,  1, 17).cwyear.should == 2007
    DateTime.civil(2008, 10, 28).cwyear.should == 2008
    DateTime.civil(2007, 12, 31).cwyear.should == 2008
    DateTime.civil(2010,  1,  1).cwyear.should == 2009
    DateTime.commercial(2008, 1, 1).cwyear.should == 2008
    DateTime.commercial(2008, 52, 1).cwyear.should == 2008
    DateTime.jd(2454782).cwyear.should == 2008
    DateTime.jd(2454832).cwyear.should == 2009
    DateTime.ordinal(2008, 1).cwyear.should == 2008
    DateTime.ordinal(2008, 359).cwyear.should == 2008
    DateTime.ordinal(2008, 366).cwyear.should == 2009
  end
end

describe "DateTime#cweek" do
  it "should be able to determine the commercial week for a date" do
    DateTime.civil(2007,  1, 17).cweek.should == 3
    DateTime.civil(2008, 10, 28).cweek.should == 44
    DateTime.civil(2007, 12, 31).cweek.should == 1
    DateTime.civil(2010,  1,  1).cweek.should == 53
    DateTime.commercial(2008, 1, 1).cweek.should == 1
    DateTime.commercial(2008, 10, 5).cweek.should == 10
    DateTime.jd(2454782).cweek.should == 46
    DateTime.jd(2454789).cweek.should == 47
    DateTime.ordinal(2008, 1).cweek.should == 1
    DateTime.ordinal(2008, 359).cweek.should == 52
    DateTime.ordinal(2008, 366).cweek.should == 1
  end
end

describe "DateTime#cwday" do
  it "should be able to determine the commercial week day for a date" do
    DateTime.civil(2007,  1, 17).cwday.should == 3
    DateTime.civil(2008, 10, 26).cwday.should == 7
    DateTime.commercial(2008, 1, 1).cwday.should == 1
    DateTime.commercial(2008, 10, 5).cwday.should == 5
    DateTime.jd(2454782).cwday.should == 2
    DateTime.jd(2454786).cwday.should == 6
    DateTime.ordinal(2008, 1).cwday.should == 2
    DateTime.ordinal(2008, 317).cwday.should == 3
  end  
end

describe "DateTime#mday and #day" do
  it "should be able to determine the day of the month for a date" do
    DateTime.civil(2007,  1, 17).mday.should == 17
    DateTime.civil(2008, 10, 28).mday.should == 28
    DateTime.civil(2007,  1, 17).day.should == 17
    DateTime.civil(2008, 10, 28).day.should == 28
    DateTime.commercial(2008, 1, 1).day.should == 31
    DateTime.commercial(2008, 52, 1).day.should == 22
    DateTime.jd(2454782).day.should == 11
    DateTime.jd(2454832).day.should == 31
    DateTime.ordinal(2008, 1).day.should == 1
    DateTime.ordinal(2008, 359).day.should == 24
    DateTime.ordinal(2008, 366).day.should == 31
  end
end

describe "DateTime#jd" do
  it "should be able to determine the julian date for a date" do
    DateTime.civil(2007,  1, 17).jd.should == 2454118
    DateTime.civil(2008, 10, 28).jd.should == 2454768
    DateTime.commercial(2008, 1, 1).jd.should == 2454466
    DateTime.commercial(2008, 52, 1).jd.should == 2454823
    DateTime.jd(2454782).jd.should == 2454782
    DateTime.jd(2454832).jd.should == 2454832
    DateTime.ordinal(2008, 1).jd.should == 2454467
    DateTime.ordinal(2008, 359).jd.should == 2454825
    DateTime.ordinal(2008, 366).jd.should == 2454832
  end 
end

describe "DateTime#ajd" do
  it "should be a float of the astronomical julian day" do
    DateTime.jd(2008).ajd.should == 2007.5
    DateTime.jd(2008, 12).ajd.should == 2008
    DateTime.jd(2008, 0, 0, 0, -0.5).ajd.should == 2008
    DateTime.jd(2008, 12, 0, 0, -0.5).ajd.should == 2008.5
  end
end

describe "DateTime#amjd" do
  it "#amjd should be a float of the astronomical julian day" do
    DateTime.jd(2008).amjd.should == -2397993.0
    DateTime.jd(2008, 12).amjd.should == -2397992.5
    DateTime.jd(2008, 0, 0, 0, -0.5).amjd.should == -2397992.5
    DateTime.jd(2008, 12, 0, 0, -0.5).amjd.should == -2397992.0
  end
end

describe "DateTime#mjd" do
  it "should be able to determine the Modified Julian day for a date" do
    DateTime.civil(2007, 1, 17).mjd.should == 54117
  end 
end

describe "DateTime#ld" do
  it "should be able to determine the Modified Julian day for a date" do
    DateTime.civil(2007, 1, 17).ld.should == 154958
  end 
end

describe "DateTime#wday" do
  it "should be able to determine the week day for a date" do
    DateTime.civil(2007,  1, 17).wday.should == 3
    DateTime.civil(2008, 10, 26).wday.should == 0
    DateTime.commercial(2008, 1, 1).wday.should == 1
    DateTime.commercial(2008, 52, 1).wday.should == 1
    DateTime.jd(2454782).wday.should == 2
    DateTime.jd(2454832).wday.should == 3
    DateTime.ordinal(2008, 1).wday.should == 2
    DateTime.ordinal(2008, 170).wday.should == 3
    DateTime.ordinal(2008, 366).wday.should == 3
  end
end

describe "DateTime#year" do
  it "should be able to determine the year for a date" do
    DateTime.civil(2007, 1, 17).year.should == 2007
    DateTime.civil(2008, 1, 17).year.should == 2008
    DateTime.commercial(2008, 1, 1).year.should == 2007
    DateTime.commercial(2008, 52, 1).year.should == 2008
    DateTime.jd(2454782).year.should == 2008
    DateTime.jd(2454833).year.should == 2009
    DateTime.ordinal(2008, 1).year.should == 2008
    DateTime.ordinal(2008, 170).year.should == 2008
    DateTime.ordinal(2008, 366).year.should == 2008
  end 
end

describe "DateTime#yday" do
  it "should be able to determine the year for a date" do
    DateTime.civil(2007,  1, 17).yday.should == 17
    DateTime.civil(2008, 10, 28).yday.should == 302
    DateTime.commercial(2008, 1, 1).yday.should == 365
    DateTime.commercial(2008, 52, 1).yday.should == 357
    DateTime.jd(2454782).yday.should == 316
    DateTime.jd(2454832).yday.should == 366
    DateTime.ordinal(2008, 1).yday.should == 1
    DateTime.ordinal(2008, 170).yday.should == 170
    DateTime.ordinal(2008, 366).yday.should == 366
  end 
end

describe "DateTime#mon and #month" do
  it "should be able to determine the month for a date" do
    DateTime.civil(2007,  1, 17).mon.should == 1
    DateTime.civil(2008, 10, 28).mon.should == 10
    DateTime.civil(2007,  1, 17).month.should == 1
    DateTime.civil(2008, 10, 28).month.should == 10
    DateTime.commercial(2008, 1, 1).mon.should == 12
    DateTime.commercial(2008, 52, 1).mon.should == 12
    DateTime.jd(2454782).mon.should == 11
    DateTime.jd(2454832).mon.should == 12
    DateTime.ordinal(2008, 1).mon.should == 1
    DateTime.ordinal(2008, 170).mon.should == 6
    DateTime.ordinal(2008, 366).mon.should == 12
  end 
end

