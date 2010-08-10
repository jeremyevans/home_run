require File.expand_path('../../spec_helper', __FILE__)

describe "Date#jd" do

  it "should be able to construct a Date object based on the Julian day" do
    Date.jd(2454482).should == Date.civil(2008, 1, 16)
  end

  it "should be able to determine the Julian day for a Date object" do
    Date.civil(2008, 1, 16).jd.should == 2454482
  end
  
end
