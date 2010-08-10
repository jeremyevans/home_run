require File.expand_path('../../spec_helper', __FILE__)

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
  end 
end

describe "DateTime#sec" do
  it "should be able to determine the second of the day" do
    DateTime.jd(2007, 1, 2, 3).sec.should == 3
  end 
end

describe "DateTime#zone" do
  it "should give the offset as a string" do
    DateTime.jd(0).zone.should == '+00:00'
    DateTime.jd(2007, 0, 0, 0, -1/24.0).zone.should == '-01:00'
  end 
end
