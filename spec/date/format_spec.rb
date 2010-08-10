require File.expand_path('../../spec_helper', __FILE__)

describe "Date formatting methods" do
  it "#asctime and #ctime should use a format similar to Time" do
    Date.new(2008, 1, 2).asctime.should == 'Wed Jan  2 00:00:00 2008'
    Date.new(2008, 1, 2).ctime.should == 'Wed Jan  2 00:00:00 2008'
  end
  
  it "#to_s should use an ISO8601 format" do
    Date.new(2008, 1, 2).to_s.should == '2008-01-02'
  end

  it "#inspect should use an ISO8601 format" do
    Date.new(2008, 1, 2).inspect.should == '#<Date 2008-01-02>'
  end
end
