require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime#day_fraction" do
  it "should be able to determine the fraction of the day" do
    DateTime.new(2008, 1, 1).day_fraction.should == 0.0
    DateTime.new(2008, 1, 1, 12).day_fraction.should == 0.5
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

describe "DateTime#offset and #utc_offset" do
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
  it "should be the fraction of a second as a fraction of the day" do
    DateTime.new(2008, 1, 1).sec_fraction.should == 0.0
    DateTime.parse('12:13:15.678900').sec_fraction.should be_close(7.85763888888889e-06, 0.0000000000001)
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

