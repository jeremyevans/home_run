require File.expand_path('../../spec_helper', __FILE__)

describe "Date formatting methods" do
  it "#asctime and #ctime should use a format similar to Time" do
    DateTime.new(2008, 1, 2, 10, 20, 30).asctime.should == 'Wed Jan  2 10:20:30 2008'
    DateTime.new(2008, 1, 2, 10, 20, 30).ctime.should == 'Wed Jan  2 10:20:30 2008'
  end
  
  it "#to_s should use an ISO8601 format" do
    DateTime.new(2008, 1, 2, 10, 20, 30, 8/24.0).to_s.should == '2008-01-02T10:20:30+08:00'
  end

  it "#inspect should use an ISO8601 format" do
    DateTime.new(2008, 1, 2, 10, 20, 30, 8/24.0).inspect.should == '#<DateTime 2008-01-02T10:20:30+08:00>'
  end
end
