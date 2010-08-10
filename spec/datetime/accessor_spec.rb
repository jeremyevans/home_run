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

