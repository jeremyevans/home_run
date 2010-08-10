require File.expand_path('../../spec_helper', __FILE__)

describe "Date.today" do
  it "should be today's date" do
    t = Time.now
    d = Date.today
    d.year.should == t.year
    d.mon.should == t.mon
    d.day.should == t.day
  end
end
